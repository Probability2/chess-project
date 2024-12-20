#pragma once

#include <iostream>
#include <vector>
#include <string>

namespace ArgumentParser {

class Flags {
  public:
    char short_parameter_;
    std::string long_parameter_;
    std::string description_;
    bool value_ = false;
    bool* valueptr_;
    bool is_valueptr_set_ = false;
    bool default_value_ = false;

    Flags(const char short_argument, const std::string& long_argument, const std::string& description);
    Flags& Default(const bool bool_value);
    Flags& StoreValue(bool& value);
};
}
