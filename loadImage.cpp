#include <ImageIO/loadImage.h>

#include <ImageIO/loadJpeg.h>
#include <ImageIO/loadPng.h>

#include <stdexcept>

namespace ImageIO_NS
{
  using ::imageview::PixelFormatGrayscale8;
  using ::imageview::PixelFormatRGB24;

  std::unique_ptr<IBitmap<PixelFormatGrayscale8>> loadImageGrayscale8(const std::filesystem::path& path)
  {
    if (path.extension() == ".png")
    {
      return loadPngGrayscale8(path);
    }
    if (path.extension() == ".jpg" or path.extension() == ".jpeg")
    {
      return loadJpegGrayscale8(path);
    }
    throw std::runtime_error("Unknown filename extension: " + path.string());
  }

  std::unique_ptr<IBitmap<PixelFormatRGB24>> loadImageRGB24(const std::filesystem::path& path)
  {
    if (path.extension() == ".png")
    {
      return loadPngRGB24(path);
    }
    if (path.extension() == ".jpg" or path.extension() == ".jpeg")
    {
      return loadJpegRGB24(path);
    }
    throw std::runtime_error("Unknown filename extension: " + path.string());
  }
}
