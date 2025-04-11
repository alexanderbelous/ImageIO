#include <ImageIO/readBinaryFile.h>

#include <fstream>

namespace ImageIO_NS
{
  Blob readBinaryFile(const std::filesystem::path& path)
  {
    const std::uintmax_t file_size = std::filesystem::file_size(path);
    std::ifstream stream(path, std::ios_base::binary);
    std::unique_ptr<std::byte[]> data = std::make_unique_for_overwrite<std::byte[]>(file_size);
    stream.read(reinterpret_cast<char*>(data.get()), file_size);
    return Blob(std::move(data), file_size);
  }
}
