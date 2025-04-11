#include <ImageIO/loadImage.h>

#include <ImageIO/BitmapImpl.h>
#include <ImageIO/readBinaryFile.h>

#include <libjpeg-turbo/src/turbojpeg.h>

#include <fstream>
#include <stdexcept>

namespace ImageIO_NS
{
  namespace
  {
    using ::imageview::PixelFormatGrayscale8;
    using ::imageview::PixelFormatRGB24;

    class TjDecompressor
    {
    public:
      TjDecompressor():
        handle_(tjInitDecompress())
      {}

      TjDecompressor(const TjDecompressor&) = delete;

      TjDecompressor(TjDecompressor&& other) noexcept:
        handle_(std::exchange(other.handle_, nullptr))
      {}
      TjDecompressor& operator=(const TjDecompressor&) = delete;

      TjDecompressor& operator=(TjDecompressor&& other)
      {
        tjDestroy(handle_);
        handle_ = std::exchange(other.handle_, nullptr);
      }

      ~TjDecompressor()
      {
        if (handle_) {
          tjDestroy(handle_);
        }
      }

      constexpr tjhandle get() const { return handle_; }

     private:
      tjhandle handle_ = nullptr;
    };

    std::unique_ptr<IBitmap<PixelFormatGrayscale8>>
    loadJpegFromMemoryGrayscale8(std::span<const std::byte> compressed_data)
    {
      TjDecompressor jpeg_decompressor;
      int width = 0;
      int height = 0;
      int jpeg_subsamp = 0;
      int jpeg_colorspace = 0;
      tjDecompressHeader3(
          jpeg_decompressor.get(),
          reinterpret_cast<const unsigned char*>(compressed_data.data()),
          static_cast<unsigned long>(compressed_data.size()), &width, &height,
          &jpeg_subsamp, &jpeg_colorspace);
      std::vector<unsigned char> buffer(width * height *
                                        PixelFormatRGB24::kBytesPerPixel);
      tjDecompress2(jpeg_decompressor.get(),
                    reinterpret_cast<const unsigned char*>(compressed_data.data()),
                    static_cast<unsigned long>(compressed_data.size()),
                    buffer.data(), width, 0 /*pitch*/, height, TJPF_GRAY,
                    TJFLAG_FASTDCT);
      return std::make_unique<BitmapImpl<PixelFormatGrayscale8>>(
          static_cast<unsigned int>(height), static_cast<unsigned int>(width),
          std::move(buffer));
    }

    std::unique_ptr<IBitmap<PixelFormatRGB24>> loadJpegFromMemoryRGB24(std::span<const std::byte> compressed_data)
    {
      TjDecompressor jpeg_decompressor;
      int width = 0;
      int height = 0;
      int jpeg_subsamp = 0;
      int jpeg_colorspace = 0;
      tjDecompressHeader3(
          jpeg_decompressor.get(),
          reinterpret_cast<const unsigned char*>(compressed_data.data()),
          static_cast<unsigned long>(compressed_data.size()), &width, &height,
          &jpeg_subsamp, &jpeg_colorspace);
      std::vector<unsigned char> buffer(width * height *
                                        PixelFormatRGB24::kBytesPerPixel);
      tjDecompress2(jpeg_decompressor.get(),
                    reinterpret_cast<const unsigned char*>(compressed_data.data()),
                    static_cast<unsigned long>(compressed_data.size()),
                    buffer.data(), width, 0 /*pitch*/, height, TJPF_RGB,
                    TJFLAG_FASTDCT);
      return std::make_unique<BitmapImpl<PixelFormatRGB24>>(
          static_cast<unsigned int>(height), static_cast<unsigned int>(width),
          std::move(buffer));
    }
  }

  std::unique_ptr<IBitmap<PixelFormatGrayscale8>> loadJpegGrayscale8(const std::filesystem::path& path)
  {
    const Blob compressed_data = readBinaryFile(path);
    return loadJpegFromMemoryGrayscale8(compressed_data.data());
  }

  std::unique_ptr<IBitmap<PixelFormatRGB24>> loadJpegRGB24(const std::filesystem::path& path)
  {
    const Blob compressed_data = readBinaryFile(path);
    return loadJpegFromMemoryRGB24(compressed_data.data());
  }
}
