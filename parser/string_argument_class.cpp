#include "string_argument_class.hpp"

namespace ArgumentParser {

StringArguments::StringArguments(const char short_argument, const std::string& long_argument, const std::string& description)
: short_parameter_(short_argument)
, long_parameter_(long_argument)
, description_(description) {
}

StringArguments& StringArguments::Default(const std::string& string_value) {
  default_value_ = string_value;
  value_ = string_value;

  return *this;
}

StringArguments& StringArguments::MultiValue(const uint64_t min_args_count) {
  min_argc_count_ = min_args_count;
  is_multi_value_ = true;

  return *this;
}

StringArguments& StringArguments::MultiValue() {
  is_multi_value_ = true;

  return *this;
}

StringArguments& StringArguments::Positional() {
  is_positional_ = true;

  return *this;
}

StringArguments& StringArguments::StoreValue(std::string& value) {
  if (is_multi_value_) {
    std::cerr << "ERROR:\nYou can store only one value.\n";
  } else {
    valueptr_ = &value;
    is_valueptr_set_ = true;
  }

  return *this;
}

StringArguments& StringArguments::StoreValues(std::vector<std::string>& values) {
  if (!is_multi_value_) {
    std::cerr << "ERROR:\nYou can store only multiple values.\n";
  } else {
    valuesptr_ = &values;
    is_valuesptr_set_ = true;
  }

  return *this;
}

bool StringArguments::IsArgSet() const {
  return !value_.has_value();
}

}
