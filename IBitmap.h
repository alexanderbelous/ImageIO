#pragma once

#include <imageview/ContinuousImageView.h>

namespace ImageIO_NS
{
  template <class PixelFormat>
  class IBitmap
  {
   public:
    virtual ~IBitmap() = default;

    virtual imageview::ContinuousImageView<PixelFormat> getView() const = 0;
  };
}
