#pragma once

#include "DisplayBoard.hpp"

#include <cinttypes>
#include <memory>

class BMPImage {
public:
  BMPImage (const int32_t height, const int32_t width);
  void SetColor(const int color_type, const uint64_t pixel_count);
  void Export(const char* path, const uint64_t byte_count);
  void FillFileHeader(uint8_t fileHeader[]);
  void FillInfoHeader(uint8_t infoHeader[]);
  void WriteIntoFile(std::ofstream& of, const uint64_t number_pixels);
  void WriteHeader(const uint8_t ind_begin, uint8_t header[], int32_t size);

private:
  int32_t iheight_;
  int32_t iwidth_;
  int32_t file_size_;
  int32_t file_offset_;
  std::unique_ptr<uint32_t> colors_;
  std::unique_ptr<uint8_t> file_header_;
  std::unique_ptr<uint8_t> info_header_;
  // int32_t* colors_;
  // uint8_t* file_header_;
  // uint8_t* info_header_;
};
