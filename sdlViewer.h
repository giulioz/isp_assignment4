#ifndef SDLVIEWER_H
#define SDLVIEWER_H

#include "bmpRepo.h"

/// @brief Displays the image contained in the given BMP repository entry using SDL. Blocking until the user closes the window
/// @param entry Pointer to the BMP repository entry containing the image data.
void bmpEntryDisplayImage(BmpRepoEntry *entry);

#endif // SDLVIEWER_H
