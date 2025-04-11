#pragma once

#include <ImageIO/IBitmap.h>

#include <stdexcept>
#include <vector>

namespace ImageIO_NS
{
  // An implementation of IBitmap, which stores the binary data in
  // std::vector<unsigned char>.
  template <class PixelFormat>
  class BitmapImpl : public IBitmap<PixelFormat>
  {
   public:
    static_assert(imageview::IsPixelFormat<PixelFormat>::value,
                  "Not a PixelFormat.");

    explicit BitmapImpl(unsigned int height, unsigned int width, std::vector<unsigned char> data):
      data_(std::move(data)),
      height_(height),
      width_(width)
    {
      if (data_.size() != height * width * PixelFormat::kBytesPerPixel)
      {
        throw std::invalid_argument("ImageIO::BitmapImpl(): wrong number of bytes in the input data.");
      }
    }

    imageview::ContinuousImageView<PixelFormat> getView() const override
    {
      return imageview::ContinuousImageView<PixelFormat>(
          height_, width_, std::as_bytes(std::span{data_}));
    }

   private:
    std::vector<unsigned char> data_;
    unsigned int height_;
    unsigned int width_;
  };
}
