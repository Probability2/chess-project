#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <string>

namespace ArgumentParser {

class StringArguments {
public:
  char short_parameter_;
  std::string long_parameter_;
  std::string description_;
  std::optional<std::string> value_;
  std::string* valueptr_;
  std::vector<std::string> values_;
  std::vector<std::string>* valuesptr_;
  bool is_valueptr_set_ = false;
  bool is_valuesptr_set_ = false;
  bool is_positional_ = false;
  bool is_multi_value_ = false;
  uint64_t min_argc_count_ = 1;
  std::optional<std::string> default_value_;

  StringArguments(const char short_argument, const std::string& long_argument, const std::string& description);
  StringArguments& Default(const std::string& string_value);
  StringArguments& StoreValue(std::string& value);
  StringArguments& StoreValues(std::vector<std::string>& values);
  StringArguments& Positional();
  StringArguments& MultiValue(const uint64_t min_args_count);
  StringArguments& MultiValue();

  bool IsArgSet() const;
};
}
