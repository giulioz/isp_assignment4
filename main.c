#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "bmpRepo.h"
#include "cleanupMgr.h"
#include "common.h"
#include "opTree.h"
#include "sdlViewer.h"
#include "strings.h"

typedef enum CommandType {
  CMD_INVALID = 0,
  CMD_HELP,
  CMD_LOAD,
  CMD_CROP,
  CMD_PLACE,
  CMD_UNDO,
  CMD_PRINT,
  CMD_SWITCH,
  CMD_TREE,
  CMD_BMPS,
  CMD_SAVE,
  CMD_QUIT
} CommandType;

/// @brief Gets the command type by parsing the first word of the command string
/// @param command the command string entered by the user in the prompt
/// @return the found CommandType, CMD_INVALID if not found
CommandType getCommandType(const char* command) {
  const char* validCommands[] = {"help", "load",  "crop",   "place",
                                 "undo", "print", "switch", "tree",
                                 "bmps", "save",  "quit"};
  int nValidCommands = sizeof(validCommands) / sizeof(validCommands[0]);

  for (int i = 0; i < nValidCommands; i++) {
    int match = 1;
    for (size_t j = 0; j < strlen(validCommands[i]) && j < strlen(command);
         j++) {
      if (command[j] == ' ' || command[j] == '\n') {
        break;
      }
      if (command[j] != validCommands[i][j]) {
        match = 0;
        break;
      }

      // TODO: properly match only full words
      // right now it's possible to match "helpme" as "help"
    }

    if (match) {
      return i + 1;
    }
  }
  return CMD_INVALID;
}

void doCommand_Help(void) {
  printf(
      "\nAvailable commands:\n help\n load <PATH>\n crop <BMP_ID> <TOP_X> "
      "<TOP_Y> <BOTTOM_X> <BOTTOM_Y>\n place <BMP_ID> <CANVAS_X> <CANVAS_Y> "
      "<BLEND_MODE>\n undo\n print\n switch <LAYER_ID>\n tree\n bmps\n save "
      "<FILE_PATH>\n quit\n\n");
}

int doCommand_Load(CleanupMgr* cleanupMgr, BmpRepo* bmpRepo,
                   const char* command) {
  // Syntax: load <PATH>

  // Parse path from command
  char* path = getNWord(command, 1, cleanupMgr);
  if (!path) {
    printf(ERROR_ARGUMENTS);
    return 0;
  }

  // Read BMP header
  BmpHeader header;
  FILE* file = bmpReadHeader(path, &header);
  if (file == NULL) {
    return 0;
  }

  // Allocate memory for bitmap entry
  BmpRepoEntry* entry =
      bmpRepo_createEmptyBmp(bmpRepo, header.width_, header.height_);
  if (entry == NULL) {
    // memory allocation failed
    fclose(file);
    return -1;
  }

  // Read pixel data
  fseek(file, header.offset_pixel_array_, SEEK_SET);
  if (fread(entry->pixelData, header.raw_bitmap_data_size_, 1, file) != 1) {
    printf("[ERROR] Failed to read pixel data!\n");
    fclose(file);
    return 0;
  }

  printf(
      "Loaded %s with ID %d and dimensions %d "
      "%d\n",
      path, entry->bmpId, entry->width, entry->height);
  fclose(file);
  return 0;
}

int doCommand_Crop(BmpRepo* bmpRepo, const char* command) {
  // Syntax: crop <BMP_ID> <TOP_X> <TOP_Y> <BOTTOM_X> <BOTTOM_Y>

  int bmpId = -1;
  int topX = -1, topY = -1, bottomX = -1, bottomY = -1;

  // Parse parameters from command
  if (sscanf(command, "crop %d %d %d %d %d", &bmpId, &topX, &topY, &bottomX,
             &bottomY) != 5) {
    printf(ERROR_ARGUMENTS);
    return 0;
  }

  BmpRepoEntry* originalBmp = bmpRepo_getBmpById(bmpRepo, bmpId);
  if (originalBmp == NULL) {
    printf(ERROR_NO_BMPID);
    return 0;
  }

  // Sanity checks
  if (topX < 0 || topY < 0 || bottomX <= topX || bottomY <= topY) {
    printf(ERROR_INVALID_RECT);
    return 0;
  }
  if (bottomX > originalBmp->width || bottomY > originalBmp->height) {
    printf(ERROR_CROP_OUTSIDE);
    return 0;
  }

  // Create new empty cropped BMP
  int croppedWidth = bottomX - topX;
  int croppedHeight = bottomY - topY;
  BmpRepoEntry* croppedBmp =
      bmpRepo_createEmptyBmp(bmpRepo, croppedWidth, croppedHeight);
  if (croppedBmp == NULL) {
    // memory allocation failed
    return -1;
  }

  // Copy image date
  for (int y = 0; y < croppedHeight; y++) {
    for (int x = 0; x < croppedWidth; x++) {
      size_t srcIdx =
          ((size_t)(topY + y) * originalBmp->width + (size_t)(topX + x)) *
          BMP_STRIDE;
      size_t destIdx = ((size_t)y * croppedWidth + (size_t)x) * BMP_STRIDE;
      croppedBmp->pixelData[destIdx + 0] = originalBmp->pixelData[srcIdx + 0];
      croppedBmp->pixelData[destIdx + 1] = originalBmp->pixelData[srcIdx + 1];
      croppedBmp->pixelData[destIdx + 2] = originalBmp->pixelData[srcIdx + 2];
      croppedBmp->pixelData[destIdx + 3] = originalBmp->pixelData[srcIdx + 3];
    }
  }

  bmpEntryDisplayImage(croppedBmp);

  printf(
      "Cropped ID %d to new ID %d with dimensions %d "
      "x %d\n",
      originalBmp->bmpId, croppedBmp->bmpId, croppedWidth, croppedHeight);
  return 0;
}

int main(int argc, char* argv[]) {
  int canvasWidth = 0, canvasHeight = 0;
  BmpRepo* bmpRepo;
  OpTreeNode* rootOpNode;
  CleanupMgr* cleanupMgr;

  //
  // Init all
  //
  cleanupMgr = cleanupMgr_init();
  if (cleanupMgr == NULL) {
    return 1;
  }

  bmpRepo = bmpRepo_init(cleanupMgr);
  if (bmpRepo == NULL) {
    cleanupMgr_cleanupAll(cleanupMgr);
    return 1;
  }

  rootOpNode = opTree_createRoot(cleanupMgr);
  if (rootOpNode == NULL) {
    cleanupMgr_cleanupAll(cleanupMgr);
    return 1;
  }

  //
  // Check cli parameters
  //
  if (argc != 3) {
    printf(ERROR_CL_PARAMS);
    return 2;
  }

  if (sscanf(argv[1], "%i", &canvasWidth) != 1) {
    printf(ERROR_CANVAS_SIZE);
    return 3;
  }
  if (sscanf(argv[2], "%i", &canvasHeight) != 1) {
    printf(ERROR_CANVAS_SIZE);
    return 3;
  }

  //
  // Prompt loop
  //
  printf(
      "\nWelcome to Image Structuring Program!\nThe canvas is %d x "
      "%d pixels.\n\n",
      canvasWidth, canvasHeight);

  int running = 1;
  while (running) {
    printf("> ");

    char* command = readStringAlloc(cleanupMgr);
    CommandType cmdType = getCommandType(command);

    switch (cmdType) {
      case CMD_INVALID:
        printf(ERROR_CMD_UNK);
        break;

      case CMD_HELP:
        doCommand_Help();
        break;

      case CMD_QUIT:
        running = 0;
        break;

      case CMD_LOAD:
        if (doCommand_Load(cleanupMgr, bmpRepo, command) == -1) {
          cleanupMgr_cleanupAll(cleanupMgr);
          return 1;
        }
        break;

      case CMD_CROP:
        if (doCommand_Crop(bmpRepo, command) == -1) {
          cleanupMgr_cleanupAll(cleanupMgr);
          return 1;
        }
        break;
    }

    cleanupMgr_freeSingle(cleanupMgr, command);
  }

  // bmpEntryDisplayImage(entry);

  //
  // Cleanup
  //
  cleanupMgr_cleanupAll(cleanupMgr);

  return 0;
}
