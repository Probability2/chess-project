#include "int_argument_class.hpp"

namespace ArgumentParser {

IntArguments::IntArguments(const char short_argument, const std::string& long_argument, const std::string& description)
: short_parameter_(short_argument)
, long_parameter_(long_argument)
, description_(description) {
}

IntArguments& IntArguments::Default(const int32_t int_value) {
  default_value_ = int_value;
  value_ = int_value;

  return *this;
}

IntArguments& IntArguments::MultiValue(const uint64_t min_args_count) {
  min_argc_count_ = min_args_count;
  is_multi_value_ = true;

  return *this;
}

IntArguments& IntArguments::MultiValue() {
  is_multi_value_ = true;

  return *this;
}

IntArguments& IntArguments::Positional() {
  is_positional_ = true;

  return *this;
}

IntArguments& IntArguments::StoreValue(int32_t& value) {
  if (is_multi_value_) {
    std::cerr << "ERROR:\nYou can store only one value.\n";
  } else {
    valueptr_ = &value;
    is_valueptr_set_ = true;
  }

  return *this;
}

IntArguments& IntArguments::StoreValues(std::vector<int32_t>& values) {
  if (!is_multi_value_) {
    std::cerr << "ERROR:\nYou can store only multiple values.\n";
  } else {
    valuesptr_ = &values;
    is_valuesptr_set_ = true;
  }

  return *this;
}
}

#include "int_argument_class.hpp"

namespace ArgumentParser {

IntArguments::IntArguments(const char short_argument, const std::string& long_argument, const std::string& description)
: short_parameter_(short_argument)
, long_parameter_(long_argument)
, description_(description) {
}

IntArguments& IntArguments::Default(const int32_t int_value) {
  default_value_ = int_value;
  value_ = int_value;

  return *this;
}

IntArguments& IntArguments::MultiValue(const uint64_t min_args_count) {
  min_argc_count_ = min_args_count;
  is_multi_value_ = true;

  return *this;
}

IntArguments& IntArguments::MultiValue() {
  is_multi_value_ = true;

  return *this;
}

IntArguments& IntArguments::Positional() {
  is_positional_ = true;

  return *this;
}

IntArguments& IntArguments::StoreValue(int32_t& value) {
  if (is_multi_value_) {
    std::cerr << "ERROR:\nYou can store only one value.\n";
  } else {
    valueptr_ = &value;
    is_valueptr_set_ = true;
  }

  return *this;
}

IntArguments& IntArguments::StoreValues(std::vector<int32_t>& values) {
  if (!is_multi_value_) {
    std::cerr << "ERROR:\nYou can store only multiple values.\n";
  } else {
    valuesptr_ = &values;
    is_valuesptr_set_ = true;
  }

  return *this;
}
}
