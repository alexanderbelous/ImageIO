#pragma once

#include <ImageIO/IBitmap.h>

#include <imageview/pixel_formats/PixelFormatGrayscale8.h>
#include <imageview/pixel_formats/PixelFormatRGB24.h>

#include <filesystem>
#include <memory>

namespace ImageIO_NS
{
  std::unique_ptr<IBitmap<imageview::PixelFormatGrayscale8>> loadImageGrayscale8(const std::filesystem::path& path);

  std::unique_ptr<IBitmap<imageview::PixelFormatRGB24>> loadImageRGB24(const std::filesystem::path& path);
}
