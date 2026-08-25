#pragma once

#include <concepts>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

namespace fs = std::filesystem;

using iterator = std::vector<uint8_t>::iterator;
using const_iterator = std::vector<uint8_t>::const_iterator;
using size_type = std::size_t;
using difference_type = std::ptrdiff_t;

