#include <ImageIO/loadPng.h>

#include <ImageIO/readBinaryFile.h>

#include <ImageIO/thirdparty/lodepng/lodepng.h>

#include <cstdlib>
#include <stdexcept>

namespace ImageIO_NS
{
  namespace
  {
    using ::imageview::ContinuousImageView;
    using ::imageview::PixelFormatGrayscale8;
    using ::imageview::PixelFormatRGB24;

    class LoadPngDeleter
    {
    public:
      void operator()(unsigned char* pointer) const { std::free(pointer); }
    };

    struct PngData
    {
      std::unique_ptr<unsigned char[], LoadPngDeleter> data;
      unsigned int height = 0;
      unsigned int width = 0;
    };

    PngData loadPngData(const std::filesystem::path& path,
                        LodePNGColorType color_type,
                        unsigned int bit_depth)
    {
      const Blob compressed_file = readBinaryFile(path);
      const std::span<const std::byte> compressed_data = compressed_file.data();
      PngData result;
      unsigned char* data_raw = nullptr;
      const unsigned int status = lodepng_decode_memory(
          &data_raw, &result.width, &result.height,
          reinterpret_cast<const unsigned char*>(compressed_data.data()),
          compressed_data.size(), color_type, bit_depth);
      result.data.reset(std::exchange(data_raw, nullptr));
      if (status != 0)
      {
        throw std::runtime_error(lodepng_error_text(status));
      }
      return result;
    }

    template <class PixelFormat>
    class PngBitmap : public IBitmap<PixelFormat>
    {
    public:
      explicit PngBitmap(unsigned int height,
                         unsigned int width,
                         std::unique_ptr<unsigned char[], LoadPngDeleter> data):
        data_(std::move(data)),
        height_(height),
        width_(width)
      {}

      ContinuousImageView<PixelFormat> getView() const override
      {
        const std::size_t size_bytes = static_cast<std::size_t>(height_) * width_ *
                                       PixelFormat::kBytesPerPixel;
        const std::span<const std::byte> data(
            reinterpret_cast<const std::byte*>(data_.get()), size_bytes);
        return ContinuousImageView<PixelFormat>(height_, width_, data);
      }

     private:
      std::unique_ptr<unsigned char[], LoadPngDeleter> data_;
      unsigned int height_ = 0;
      unsigned int width_ = 0;
    };
  }

  std::unique_ptr<IBitmap<PixelFormatGrayscale8>> loadPngGrayscale8(const std::filesystem::path& path)
  {
    static_assert(PixelFormatGrayscale8::kBytesPerPixel == 1,
                  "This function loads an 8-bit grayscale image. "
                  "PixelFormatGrayscale8::kBytesPerPixel should equal 1.");
    PngData png = loadPngData(path, LodePNGColorType::LCT_GREY, 8);
    return std::make_unique<PngBitmap<PixelFormatGrayscale8>>(png.height, png.width, std::move(png.data));
  }

  std::unique_ptr<IBitmap<PixelFormatRGB24>> loadPngRGB24(const std::filesystem::path& path)
  {
    static_assert(PixelFormatRGB24::kBytesPerPixel == 3,
                  "This function loads a 24-bit RGB image. "
                  "PixelFormatRGB24::kBytesPerPixel should equal 3.");
    PngData png = loadPngData(path, LodePNGColorType::LCT_RGB, 8);
    return std::make_unique<PngBitmap<PixelFormatRGB24>>(png.height, png.width, std::move(png.data));
  }
}
