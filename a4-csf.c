#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp.h"
#include "bmpRepo.h"
#include "common.h"
#include "memoryMgr.h"
#include "opTree.h"
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

      // This is very broken, fix!
    }

    if (match) {
      return i + 1;
    }
  }
  return CMD_INVALID;
}

// Implements the "load" command
int doCommand_Load(MemoryMgr* memoryMgr, BmpRepo* bmpRepo,
                   const char* command) {
  // Syntax: load <PATH>

  // Parse path from command
  char* path = getNWord(command, 1, memoryMgr);
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
  // We go line by line to flip the image vertically (since BMPs are stored
  // upside-down)
  fseek(file, header.offset_pixel_array_, SEEK_SET);
  for (int y = header.height_ - 1; y >= 0; y--) {
    size_t bytesToRead = header.width_ * BMP_STRIDE;
    if (fread(&entry->pixelData[y * header.width_ * BMP_STRIDE], 1, bytesToRead,
              file) != bytesToRead) {
      printf("[ERROR] Failed to read pixel data!\n");
      fclose(file);
      return 0;
    }
  }

  printf(
      "Loaded %s with ID %d and dimensions %d "
      "%d\n",
      path, entry->bmpId, entry->width, entry->height);

  fclose(file);
  memoryMgr_free(memoryMgr, path);
  return 0;
}

// Implements the "crop" command
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

  // Apparently coordinates are 1-based (??)
  topX -= 1;
  topY -= 1;

  // Get original BMP from repo
  BmpRepoEntry* originalBmp = bmpRepo_getBmpById(bmpRepo, bmpId);
  if (originalBmp == NULL) {
    printf(ERROR_NO_BMPID);
    return 0;
  }

  // Sanity checks
  if (bottomX > originalBmp->width || bottomY > originalBmp->height) {
    printf(ERROR_CROP_OUTSIDE);
    return 0;
  }
  if (topX < 0 || topY < 0 || bottomX <= topX || bottomY <= topY) {
    printf(ERROR_INVALID_RECT);
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

  printf(
      "Cropped ID %d to new ID %d with dimensions %d "
      "x %d\n",
      originalBmp->bmpId, croppedBmp->bmpId, croppedWidth, croppedHeight);
  return 0;
}

// Implements the "place" command
int doCommand_Place(BmpRepo* bmpRepo, OpTree* opTree, const char* command,
                    int canvasWidth, int canvasHeight) {
  // Syntax: place <BMP_ID> <CANVAS_X> <CANVAS_Y> <BLEND_MODE>

  int bmpId = -1;
  int canvasX = -1, canvasY = -1;
  char blendModeChar = 0;
  BlendMode blendMode = 0;

  // Parse parameters from command
  if (sscanf(command, "place %d %d %d %c", &bmpId, &canvasX, &canvasY,
             &blendModeChar) != 4) {
    printf(ERROR_ARGUMENTS);
    return 0;
  }
  blendMode = (BlendMode)blendModeChar;

  // Apparently coordinates are 1-based (??)
  canvasX -= 1;
  canvasY -= 1;

  // Get BMP entry from repo
  BmpRepoEntry* bmpEntry = bmpRepo_getBmpById(bmpRepo, bmpId);
  if (bmpEntry == NULL) {
    printf(ERROR_NO_BMPID);
    return 0;
  }

  // Sanity checks
  if (canvasX < 0 || canvasY < 0 || canvasX >= canvasWidth ||
      canvasY >= canvasHeight) {
    printf(ERROR_CANVAS_COORDS);
    return 0;
  }
  if (canvasX + bmpEntry->width > canvasWidth ||
      canvasY + bmpEntry->height > canvasHeight) {
    printf(ERROR_BMP_FIT);
    return 0;
  }
  if (blendMode != BLEND_NORMAL && blendMode != BLEND_MUL &&
      blendMode != BLEND_SUB) {
    printf(ERROR_INV_BLENDMODE);
    return 0;
  }

  // Create new layer
  OpTreeNode* newOpNode = opTree_appendNewToCurrent(
      opTree, bmpId, bmpEntry, canvasX, canvasY, blendMode);
  if (newOpNode == NULL) {
    // memory allocation failed
    return -1;
  }

  printf("Switched to layer %d\n", newOpNode->layerId);

  return 0;
}

// Implements the "undo" command
void doCommand_Undo(OpTree* opTree) {
  if (!opTree_undo(opTree)) {
    printf("Switched to layer %d\n", opTree->current->layerId);
  } else {
    printf(ERROR_ALREADY_ROOT);
  }
}

// Implements the "print" command
int doCommand_Print(BmpRepo* bmpRepo, OpTree* opTree, int canvasWidth,
                    int canvasHeight) {
  uint8_t* buffer =
      opTree_renderCurrent(opTree, bmpRepo, canvasWidth, canvasHeight);
  if (buffer == NULL) {
    return -1;
  }

  // Cols header
  printf("   ");
  for (int x = 0; x < canvasWidth; x++) {
    printf(" %02d", x + 1);
  }
  printf("\n");

  // Body
  for (int y = 0; y < canvasHeight; y++) {
    printf("%02d|", y + 1);
    for (int x = 0; x < canvasWidth; x++) {
      uint8_t* pixel = &buffer[(y * canvasWidth + x) * BMP_STRIDE];
      printf("\033[38;2;%d;%d;%dm███\033[0m", pixel[2], pixel[1], pixel[0]);
    }
    printf("|\n");
  }

  // Footer
  printf("  ");
  for (int x = 0; x < canvasWidth * 3 + 2; x++) {
    printf("-");
  }
  printf("\n");

  memoryMgr_free(opTree->mgr, buffer);

  return 0;
}

// Implements the "switch" command
void doCommand_Switch(OpTree* opTree, const char* command) {
  // Syntax: switch <LAYER_ID>

  int layerId = -1;

  // Parse parameters from command
  if (sscanf(command, "switch %d", &layerId) != 1) {
    printf(ERROR_ARGUMENTS);
    return;
  }

  if (!opTree_switch(opTree, layerId)) {
    printf("Switched to layer %d\n", layerId);
  } else {
    printf(ERROR_NO_LAYER);
  }
}

// Implements the "bmps" command
void doCommand_DisplayBmps(BmpRepo* bmpRepo) {
  for (int i = 0; i < bmpRepo->lastBmpId; i++) {
    BmpRepoEntry* entry = bmpRepo->entries[i];
    assert(entry != NULL);
    printf("BMP %d has dimensions %d x %d\n", entry->bmpId, entry->width,
           entry->height);
  }
}

// Implements the "save" command
int doCommand_Save(OpTree* opTree, BmpRepo* bmpRepo, const char* command,
                   int canvasWidth, int canvasHeight) {
  // Syntax: save <FILE_PATH>

  char* path = getNWord(command, 1, opTree->mgr);
  if (!path) {
    printf(ERROR_ARGUMENTS);
    return 0;
  }

  uint8_t* buffer =
      opTree_renderCurrent(opTree, bmpRepo, canvasWidth, canvasHeight);
  if (buffer == NULL) {
    return -1;
  }

  BmpHeader header;
  FILE* file = bmpWriteHeader(path, &header, canvasWidth, canvasHeight);
  if (file == NULL) {
    return 0;
  }

  for (int y = canvasHeight - 1; y >= 0; y--)  {
    fwrite(buffer + y * canvasWidth * BMP_STRIDE, BMP_STRIDE * canvasWidth, 1, file);
  }

  fclose(file);
  printf("Successfully saved image to %s\n", path);
  memoryMgr_free(opTree->mgr, path);
  memoryMgr_free(opTree->mgr, buffer);
  return 0;
}

int main(int argc, char* argv[]) {
  int canvasWidth = 0, canvasHeight = 0;
  BmpRepo* bmpRepo = NULL;
  OpTree* opTree = NULL;
  MemoryMgr* memoryMgr = NULL;

  //
  // Init all
  //
  memoryMgr = memoryMgr_init();
  if (memoryMgr == NULL) {
    return 1;
  }

  bmpRepo = bmpRepo_init(memoryMgr);
  if (bmpRepo == NULL) {
    memoryMgr_cleanupAll(memoryMgr);
    return 1;
  }

  opTree = opTree_init(memoryMgr);
  if (opTree == NULL) {
    memoryMgr_cleanupAll(memoryMgr);
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
    // Read and parse command
    printf(" > ");
    char* command = readStringAlloc(memoryMgr);
    CommandType cmdType = getCommandType(command);

    switch (cmdType) {
      case CMD_INVALID:
        printf(ERROR_CMD_UNK);
        break;

      case CMD_HELP:
        printf(
            "\nAvailable commands:\n help\n load <PATH>\n crop <BMP_ID> "
            "<TOP_X> "
            "<TOP_Y> <BOTTOM_X> <BOTTOM_Y>\n place <BMP_ID> <CANVAS_X> "
            "<CANVAS_Y> "
            "<BLEND_MODE>\n undo\n print\n switch <LAYER_ID>\n tree\n bmps\n "
            "save "
            "<FILE_PATH>\n quit\n\n");
        break;

      case CMD_QUIT:
        running = 0;
        break;

      case CMD_LOAD:
        if (doCommand_Load(memoryMgr, bmpRepo, command) == -1) {
          memoryMgr_cleanupAll(memoryMgr);
          return 1;
        }
        break;

      case CMD_CROP:
        if (doCommand_Crop(bmpRepo, command) == -1) {
          memoryMgr_cleanupAll(memoryMgr);
          return 1;
        }
        break;

      case CMD_PLACE:
        if (doCommand_Place(bmpRepo, opTree, command, canvasWidth,
                            canvasHeight) == -1) {
          memoryMgr_cleanupAll(memoryMgr);
          return 1;
        }
        break;

      case CMD_UNDO:
        doCommand_Undo(opTree);
        break;

      case CMD_PRINT:
        if (doCommand_Print(bmpRepo, opTree, canvasWidth, canvasHeight) == -1) {
          memoryMgr_cleanupAll(memoryMgr);
          return 1;
        }
        break;

      case CMD_SWITCH:
        doCommand_Switch(opTree, command);
        break;

      case CMD_BMPS:
        doCommand_DisplayBmps(bmpRepo);
        break;

      case CMD_TREE:
        opTree_printRecursive(opTree->root, 0);
        break;

      case CMD_SAVE:
        if (doCommand_Save(opTree, bmpRepo, command, canvasWidth,
                           canvasHeight) == -1) {
          memoryMgr_cleanupAll(memoryMgr);
          return 1;
        }
        break;
    }

    command = memoryMgr_free(memoryMgr, command);
  }

  //
  // Cleanup
  //
  memoryMgr_cleanupAll(memoryMgr);

  return 0;
}
