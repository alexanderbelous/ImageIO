#include <ImageIO/saveImage.h>

#include <lodepng/lodepng.h>

#include <stdexcept>

namespace ImageIO_NS
{
  using ::imageview::ContinuousImageView;
  using ::imageview::PixelFormatGrayscale8;
  using ::imageview::PixelFormatRGB24;
  using ::imageview::PixelFormatRGBA32;

  void saveImage(ContinuousImageView<PixelFormatGrayscale8> image, const std::filesystem::path& path)
  {
    unsigned int status = lodepng_encode_file(
        path.string().c_str(),
        reinterpret_cast<const unsigned char*>(image.data().data()),
        image.width(), image.height(), LodePNGColorType::LCT_GREY, 8);
    if (status != 0)
    {
      throw std::runtime_error(lodepng_error_text(status));
    }
  }

  void saveImage(ContinuousImageView<PixelFormatRGB24> image, const std::filesystem::path& path)
  {
    unsigned int status = lodepng_encode_file(
        path.string().c_str(),
        reinterpret_cast<const unsigned char*>(image.data().data()),
        image.width(), image.height(), LodePNGColorType::LCT_RGB, 8);
    if (status != 0)
    {
      throw std::runtime_error(lodepng_error_text(status));
    }
  }

  void saveImage(ContinuousImageView<PixelFormatRGBA32> image, const std::filesystem::path& path)
  {
    unsigned int status = lodepng_encode_file(
        path.string().c_str(),
        reinterpret_cast<const unsigned char*>(image.data().data()),
        image.width(), image.height(), LodePNGColorType::LCT_RGBA, 8);
    if (status != 0)
    {
      throw std::runtime_error(lodepng_error_text(status));
    }
  }
}
