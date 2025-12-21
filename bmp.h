//----------------------------------------------------------------------------------------------------------------------
/// Contains the definition of the BMP Header layout used in this assignment
/// as well as utility function(s) for BMPs
///
/// Author: Tutors
//----------------------------------------------------------------------------------------------------------------------

#ifndef A4_CSF_BMP_H
#define A4_CSF_BMP_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
  uint8_t b_;
  uint8_t m_;
  uint32_t total_size;
  uint32_t unused_;
  uint32_t offset_pixel_array_;
  uint32_t number_of_bytes_in_header_;
  int32_t width_;
  int32_t height_;
  uint16_t planes_;
  uint16_t number_of_bits_per_pixel_;
  uint32_t compression_;
  uint32_t raw_bitmap_data_size_;
  int32_t horizontal_resolution_;
  int32_t vertical_resolution_;
  uint32_t colors_in_palette_;
  uint32_t important_colors_;
  uint32_t red_channel_mask_;
  uint32_t green_channel_mask_;
  uint32_t blue_channel_mask_;
  uint32_t alpha_channel_mask_;
  uint32_t color_space_type_;
  uint8_t endpoints_[36];
  uint32_t red_gamma_;
  uint32_t green_gamma_;
  uint32_t blue_gamma_;
} BmpHeader;
#pragma pack(pop)

//----------------------------------------------------------------------------------------------------------------------
/// @brief Takes given BmpHeader pointer and fills in default values into that struct.
/// @param header Pointer to the BmpHeader that needs to be filled
/// @param width Width of the bmp
/// @param height Height of the bmp
void fillBmpHeaderDefaultValues(BmpHeader* header, int width, int height);

#endif //A4_CSF_BMP_H
