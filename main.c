#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmpRepo.h"
#include "cleanupMgr.h"
#include "consoleUtils.h"
#include "opTree.h"
#include "sdlViewer.h"

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

CommandType getCommandType(const char* command) {
  const char* validCommands[] = {"help", "load",  "crop",   "place",
                                 "undo", "print", "switch", "tree",
                                 "bmps", "save",  "quit"};
  int nValidCommands = sizeof(validCommands) / sizeof(validCommands[0]);

  for (int i = 0; i < nValidCommands; i++) {
    if (strcmp(command, validCommands[i]) == 0) {
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
    printf("[ERROR] Invalid amount of command line parameters!\n");
    return 2;
  }

  if (sscanf(argv[1], "%i", &canvasWidth) != 1) {
    printf("[ERROR] Invalid canvas size specified!\n");
    return 3;
  }
  if (sscanf(argv[2], "%i", &canvasHeight) != 1) {
    printf("[ERROR] Invalid canvas size specified!\n");
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
        printf("[ERROR] Command unknown!\n");
        break;

      case CMD_HELP:
        doCommand_Help();
        break;

      case CMD_QUIT:
        running = 0;
        break;
    }

    cleanupMgr_freeSingle(cleanupMgr, command);
  }

  // BmpRepoEntry* entry = bmpRepo_loadFromFile(repo,
  // "images/city_highres.bmp"); if (entry == NULL) {
  //   printf("error: failed to load BMP from file\n");
  //   bmpRepo_free(repo);
  //   return 1;
  // }

  // bmpEntryDisplayImage(entry);

  //
  // Cleanup
  //
  cleanupMgr_cleanupAll(cleanupMgr);

  return 0;
}
