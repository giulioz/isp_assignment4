#include "consoleUtils.h"

#include <stdio.h>
#include <stdlib.h>

char* readStringAlloc(void) {
  size_t size = 1;
  size_t len = 0;
  char* str = (char*)calloc(size, sizeof(char));
  if (str == NULL) {
    return NULL;
  }

  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    str[len++] = (char)c;
    if (len == size) {
      size += 1;
      char* newStr = (char*)realloc(str, size);
      if (newStr == NULL) {
        free(str);
        return NULL;
      }
      str = newStr;
    }
  }
  str[len] = '\0';
  return str;
}
