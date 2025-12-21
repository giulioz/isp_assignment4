//----------------------------------------------------------------------------------------------------------------------
/// Contains utility function(s) for BMPs
///
/// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------

#include "bmp.h"

#include <stddef.h>

void fillBmpHeaderDefaultValues(BmpHeader* header, int width, int height)
{
  const size_t size_of_header = sizeof(BmpHeader);
  const BmpHeader null = {0};
  *header = null;
  header->b_ = 'B';
  header->m_ = 'M';
  header->total_size = size_of_header + width * height * 4;
  header->width_ = width;
  header->height_ = height;
  header->offset_pixel_array_ = size_of_header;
  header->number_of_bytes_in_header_ = 108;
  header->planes_ = 1;
  header->number_of_bits_per_pixel_ = 32;
  header->compression_ = 3; // BI_BITFIELDS
  header->raw_bitmap_data_size_ = 32;
  header->horizontal_resolution_ = 2835;
  header->vertical_resolution_ = 2835;
  header->colors_in_palette_ = 0;
  header->important_colors_ = 0;
  header->red_channel_mask_ = 0x00FF0000;
  header->green_channel_mask_ = 0x0000FF00;
  header->blue_channel_mask_ = 0x000000FF;
  header->alpha_channel_mask_ = 0xFF000000;
  header->color_space_type_ = 0x73524742; // sRGB color space
}
