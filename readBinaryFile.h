#pragma once

#include <cstddef>
#include <filesystem>
#include <memory>
#include <span>
#include <utility>

namespace ImageIO_NS
{
  class Blob
  {
  public:
    constexpr Blob() noexcept = default;

    Blob(std::unique_ptr<std::byte[]> data, std::size_t size):
      data_(std::move(data)),
      size_(size)
    {}

    Blob(const Blob&) = delete;

    Blob(Blob&& other) noexcept:
      data_(std::move(other.data_)),
      size_(std::exchange(other.size_, 0))
    {}

    Blob& operator=(const Blob&) = delete;

    Blob& operator=(Blob&& other) noexcept
    {
      data_ = std::move(other.data_);
      size_ = std::exchange(other.size_, 0);
      return *this;
    }

    ~Blob() = default;

    std::span<std::byte> data() noexcept
    {
      return std::span<std::byte>(data_.get(), static_cast<std::size_t>(size_));
    }

    std::span<const std::byte> data() const noexcept
    {
      return std::span<const std::byte>(data_.get(), static_cast<std::size_t>(size_));
    }

   private:
    std::unique_ptr<std::byte[]> data_;
    std::size_t size_ = 0;
  };

  Blob readBinaryFile(const std::filesystem::path& path);
}
