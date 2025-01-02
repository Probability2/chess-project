#include "flag_class.hpp"

namespace ArgumentParser {

Flags::Flags(const char short_argument, const std::string& long_argument, const std::string& description)
: short_parameter_(short_argument)
, long_parameter_(long_argument)
, description_(description) {
}

Flags& Flags::Default(const bool bool_value) {
  default_value_ = bool_value;
  value_ = bool_value;

  return *this;
}

Flags& Flags::StoreValue(bool& value) {
  valueptr_ = &value;
  is_valueptr_set_ = true;

  return *this;
}
}
