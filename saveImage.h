#pragma once

#include <imageview/ContinuousImageView.h>
#include <imageview/pixel_formats/PixelFormatGrayscale8.h>
#include <imageview/pixel_formats/PixelFormatRGB24.h>
#include <imageview/pixel_formats/PixelFormatRGBA32.h>

#include <filesystem>

namespace ImageIO_NS
{
  void saveImage(imageview::ContinuousImageView<imageview::PixelFormatGrayscale8> image,
                 const std::filesystem::path& path);

  void saveImage(imageview::ContinuousImageView<imageview::PixelFormatRGB24> image,
                 const std::filesystem::path& path);

  void saveImage(imageview::ContinuousImageView<imageview::PixelFormatRGBA32> image,
                 const std::filesystem::path& path);
}
