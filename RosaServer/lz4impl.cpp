#include "lz4impl.h"

#include <stdexcept>

namespace Lua {
namespace lz4 {
std::string _compress(std::string_view input) {
	int compressedSize = LZ4_compressBound(input.size());
	char* compressed = new char[compressedSize];

	int status = LZ4_compress_default(reinterpret_cast<const char*>(input.data()),
	                                  compressed, input.size(), compressedSize);
	if (status == 0) {
		throw std::runtime_error("lz4 compression failed");
	}

	std::string compressedString(reinterpret_cast<const char*>(compressed),
	                             compressedSize);

	delete[] compressed;
	return compressedString;
}

std::string _uncompress(std::string_view compressed, int uncompressedSize) {
	char* uncompressed = new char[uncompressedSize];

	int status =
	    LZ4_decompress_safe(reinterpret_cast<const char*>(compressed.data()),
	                        uncompressed, compressed.size(), uncompressedSize);
	if (status < 0) {
		throw std::runtime_error("lz4 decompress failed");
	}

	std::string uncompressedString(reinterpret_cast<const char*>(uncompressed),
	                               uncompressedSize);

	delete[] uncompressed;
	return uncompressedString;
}
}  // namespace lz4
}  // namespace Lua
