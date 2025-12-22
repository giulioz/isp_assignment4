#include "common.h"

#include <stdio.h>
#include <stdlib.h>

#include "strings.h"

char* readStringAlloc(MemoryMgr* mgr) {
  size_t size = 1;
  size_t len = 0;
  char* str = (char*)memoryMgr_malloc(mgr, size * sizeof(char));
  if (str == NULL) {
    return NULL;
  }

  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    str[len++] = (char)c;
    if (len == size) {
      size += 1;
      char* newStr = (char*)memoryMgr_realloc(mgr, str, size * sizeof(char));
      if (newStr == NULL) {
        return NULL;
      }
      str = newStr;
    }
  }
  str[len] = '\0';
  return str;
}

char* getNWord(const char* str, int n, MemoryMgr* mgr) {
  int currentWord = 0;
  size_t startIdx = 0;
  size_t endIdx = 0;
  size_t i = 0;

  // Find the start and end indices of the nth word
  while (str[i] != '\0') {
    // Skip leading spaces
    while (str[i] == ' ' && str[i] != '\0') {
      i++;
    }
    if (str[i] == '\0') {
      break;
    }

    if (currentWord == n) {
      startIdx = i;
      // Find the end of the word
      while (str[i] != ' ' && str[i] != '\0') {
        i++;
      }
      endIdx = i;
      break;
    } else {
      // Move to the next word
      while (str[i] != ' ' && str[i] != '\0') {
        i++;
      }
      currentWord++;
    }
  }

  // If the nth word was not found
  if (currentWord < n) {
    return NULL;
  }

  // Allocate memory for the nth word
  size_t wordLength = endIdx - startIdx;
  char* word = (char*)memoryMgr_malloc(mgr, (wordLength + 1) * sizeof(char));
  if (word == NULL) {
    return NULL;
  }

  // Copy the nth word into the allocated memory
  for (size_t j = 0; j < wordLength; j++) {
    word[j] = str[startIdx + j];
  }
  word[wordLength] = '\0';

  return word;
}

FILE* bmpReadHeader(const char* filename, BmpHeader* header) {
  FILE* file = fopen(filename, "rb");
  if (file == NULL) {
    printf(ERROR_NO_FILE);
    return NULL;
  }

  if (fread(header, sizeof(BmpHeader), 1, file) != 1) {
    printf(ERROR_INVALID_FILE);
    fclose(file);
    return NULL;
  }

  int isValidBmp =
      (header->b_ == 'B' && header->m_ == 'M' &&
       header->number_of_bits_per_pixel_ == 32 && header->compression_ == 3);
  if (!isValidBmp) {
    printf(ERROR_INVALID_FILE);
    fclose(file);
    return NULL;
  }

  return file;
}

FILE* bmpWriteHeader(const char* filename, BmpHeader* header, int width, int height) {
  FILE* file = fopen(filename, "wb");
  if (file == NULL) {
    printf(ERROR_FILEPATH);
    return NULL;
  }

  fillBmpHeaderDefaultValues(header, width, height);

  if (fwrite(header, sizeof(BmpHeader), 1, file) != 1) {
    printf("[ERROR] Failed to write BMP header!\n");
    fclose(file);
    return NULL;
  }

  return file;
}
