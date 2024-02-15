#pragma once
#include <string>

#include "lz4.h"

namespace Lua {
namespace lz4 {
std::string _compress(std::string_view input);
std::string _uncompress(std::string_view compressed, int uncompressedSize);
}  // namespace lz4
}  // namespace Lua
