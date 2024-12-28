#include "ArgParser.hpp"

namespace ArgumentParser {

ArgParser::ArgParser(const std::string& name)
: name_parser_(name) {
}

bool ArgParser::Help() const {
  return is_help_set_;
}

void ArgParser::BuildStringArgumentDescription(StringArguments* string_argument) {
  string_argument->description_ += " [";
  bool was_begin_description = false;
  if (string_argument->is_multi_value_) {
    string_argument->description_ += ("multi-value, min_argc_count=" + std::to_string(string_argument->min_argc_count_));
    was_begin_description = true;
    if (string_argument->is_valuesptr_set_) {
      string_argument->description_ += ", there is stored values";
    }
  } else if (string_argument->is_valueptr_set_) {
    if (was_begin_description) {
      string_argument->description_ += ", ";
    }
    string_argument->description_ += "there is stored value";
    was_begin_description = true;
  }
  if (string_argument->default_value_.has_value()) {
    if (was_begin_description) {
      string_argument->description_ += ", ";
    }
    string_argument->description_ += ("default=" + string_argument->default_value_.value());
    was_begin_description = true;
  }
  if (string_argument->is_positional_) {
    if (was_begin_description) {
      string_argument->description_ += ", ";
    }
    string_argument->description_ += "positional arguments";
  }
  string_argument->description_ += "]";
}

void ArgParser::BuildIntArgumentDescription(IntArguments* int_argument) {
  int_argument->description_ += " [";
  bool was_begin_description = false;
  if (int_argument->is_multi_value_) {
    int_argument->description_ += ("multi-value, min_argc_count=" + std::to_string(int_argument->min_argc_count_));
    was_begin_description = true;
    if (int_argument->is_valuesptr_set_) {
      int_argument->description_ += ", there is stored values";
    }
  } else if (int_argument->is_valueptr_set_) {
    if (was_begin_description) {
      int_argument->description_ += ", ";
    }
    int_argument->description_ += "there is stored value";
    was_begin_description = true;
  }
  if (int_argument->default_value_.has_value()) {
    if (was_begin_description) {
      int_argument->description_ += ", ";
    }
    int_argument->description_ += ("default=" + std::to_string(int_argument->default_value_.value()));
    was_begin_description = true;
  }
  if (int_argument->is_positional_) {
    if (was_begin_description) {
      int_argument->description_ += ", ";
    }
    int_argument->description_ += ("positional arguments");
  }
  int_argument->description_ += "]";
}

void ArgParser::BuildFlagDescription(Flags* flag) {
  bool was_begin_description = false;
  flag->description_ += " [";
  if (flag->is_valueptr_set_) {
    flag->description_ += "there is stored value";
  }
  if (!was_begin_description) {
    flag->description_ += ", ";
  }
  flag->description_ += ("default=" + std::to_string(flag->default_value_) + "]");
}

void ArgParser::WriteDescription(const char short_parameter,
                                 const std::string& long_parameter,
                                 const std::string& description) {
  if (short_parameter != '\0') {
    description_parser_ += "-";
    description_parser_ += short_parameter;
  } else {
    description_parser_ += "  ";
  }
  if (!long_parameter.empty()) {
    description_parser_ += (", --" + long_parameter + ", ");
  } else {
    description_parser_ += "  ";
  }
  description_parser_ += (description + "\n");
}

void ArgParser::AddHelp(const char short_argument, const std::string& long_argument, const std::string& description) {
  if (short_argument != '\0') {
    help_parser_.short_argument_help_ = short_argument;
  }
  if (!long_argument.empty()) {
    help_parser_.long_argument_help_ = long_argument;
  }
  if (!description.empty()) {
    help_parser_.description_ = description;
  }
}

std::string ArgParser::HelpDescription() {
  description_parser_ = name_parser_ + "\n";
  description_parser_ += help_parser_.description_ + "\n";
  if (!string_arguments_.size()) {
    description_parser_ += "There is no string arguments\n";
  } else {
    description_parser_ += "String Arguments:\n";
    for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
      BuildStringArgumentDescription(&string_arguments_[i]);
      WriteDescription(string_arguments_[i].short_parameter_,
                       string_arguments_[i].long_parameter_,
                       string_arguments_[i].description_);
    }
  }
  if (!int_arguments_.size()) {
    description_parser_ += "There is no int arguments\n";
  } else {
    description_parser_ += "Int Arguments:\n";
    for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
      BuildIntArgumentDescription(&int_arguments_[i]);
      WriteDescription(int_arguments_[i].short_parameter_,
                       int_arguments_[i].long_parameter_,
                       int_arguments_[i].description_);
    }
  }
  if (!bool_arguments_.size()) {
    description_parser_ += "There is no flags\n";
  } else {
    description_parser_ += "Flags:\n";
    for (std::size_t i = 0; i < bool_arguments_.size(); ++i) {
      BuildFlagDescription(&bool_arguments_[i]);
      WriteDescription(bool_arguments_[i].short_parameter_,
                       bool_arguments_[i].long_parameter_,
                       bool_arguments_[i].description_);
    }
  }

  return description_parser_;
}

bool ArgParser::IsArgumentsCorrect() const {
  for (const auto& argument: string_arguments_) {
    if ((argument.is_multi_value_ && argument.values_.size() < argument.min_argc_count_) ||
        (!argument.value_.has_value() && !argument.is_multi_value_)) {
      return false;
    }
  }
  for (const auto& argument: int_arguments_) {
    if ((argument.is_multi_value_ && argument.values_.size() < argument.min_argc_count_) ||
        (!argument.value_.has_value() && !argument.is_multi_value_)) {
      return false;
    }
  }

  return true;
}
  
bool ArgParser::CheckShortArgument(const std::string_view& parser_string) {
  for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
    if (string_arguments_[i].short_parameter_ == parser_string[1] &&
        string_arguments_[i].value_.has_value() &&
        !string_arguments_[i].is_multi_value_) {
      std::cerr << "ERROR:\nThere is no multi-value\nIt is not possible to have multiple values.\n";
      return false;
    }
    if (string_arguments_[i].short_parameter_ == parser_string[1]) {
      string_arguments_[i].value_ = parser_string.substr(3, std::string_view::npos);
      if (string_arguments_[i].is_valueptr_set_) {
        *string_arguments_[i].valueptr_ = string_arguments_[i].value_.value();
      }
      string_arguments_[i].values_.push_back(string_arguments_[i].value_.value());
      if (string_arguments_[i].is_valuesptr_set_) {
        string_arguments_[i].valuesptr_->push_back(string_arguments_[i].value_.value());
      }
      return true;
    }
  }
  for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
    if (int_arguments_[i].short_parameter_ == parser_string[1] &&
        int_arguments_[i].value_.has_value() &&
        !int_arguments_[i].is_multi_value_) {
      std::cerr << "ERROR:\nThere is no multi-value\nIt is not possible to have multiple values.\n";
      return false;
    }
    if (int_arguments_[i].short_parameter_ == parser_string[1]) {
      try {
        std::string str = static_cast<std::string>(parser_string.substr(3, std::string_view::npos));
        size_t len = str.length();
        size_t* lenptr = &len;
        int_arguments_[i].value_ = std::stoi(str, lenptr);
        if (*lenptr != str.length()) {
          std::cerr << "ERROR:\nThere should be an integer argument.\n";
          return false;
        }
        if (int_arguments_[i].is_valueptr_set_) {
          *int_arguments_[i].valueptr_ = int_arguments_[i].value_.value();
        }
        if (int_arguments_[i].is_valuesptr_set_) {
          int_arguments_[i].valuesptr_->push_back(int_arguments_[i].value_.value());
        }
        return true;
      } catch (const std::invalid_argument&) {
        std::cerr << "ERROR:\nThere should be an integer argument.\n";
        return false;
      } catch (const std::out_of_range&) {
        std::cerr << "ERROR:\nOut of range in the integer argument.\n";
        return false;
      }
    }
  }

  return false;
}

bool ArgParser::CheckShortFlags(const std::string& parser_string) {
  if (parser_string[1] == help_parser_.short_argument_help_ && parser_string.length() == 2) {
    is_help_set_ = true;
    return true;
  }
  if (parser_string.find('=') != std::string::npos) {
    std::cerr << "ERROR\nIncorrect input for flags.\n";
    return false;
  }
  int32_t number_parameters = 0;
  for (std::size_t i = 1; i < parser_string.length(); ++i) {
    for (std::size_t j = 0; j < bool_arguments_.size(); ++j) {
      if (bool_arguments_[j].short_parameter_ == parser_string[i]) {
        bool_arguments_[j].value_ = true;
        if (bool_arguments_[j].is_valueptr_set_) {
          *bool_arguments_[j].valueptr_ = &bool_arguments_[j].value_;
        }
        number_parameters++;
      }
    }
  }
  if (number_parameters == parser_string.length() - 1) {
    return true;
  }
  std::cerr << "ERROR:\nShort flag has not found.\n";

  return false;
}

bool ArgParser::CheckLongArgument(const std::string_view& parser_string) {
  if (!help_parser_.long_argument_help_.empty()) {
    if (!help_parser_.long_argument_help_.compare(parser_string.substr(2, std::string_view::npos))) {
      is_help_set_ = true;
      return true;
    }
  }
  std::string value_parameter;
  if (parser_string.find('=') != std::string::npos) {
    size_t ind = parser_string.find('=');
    std::string_view parameter = parser_string.substr(2, ind - 2);
    bool is_parameter_set = false;
    for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
      if (string_arguments_[i].long_parameter_ == parameter) {
        is_parameter_set = true;
      }
    }
    for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
      if (int_arguments_[i].long_parameter_ == parameter) {
        is_parameter_set = true;
      }
    }
    if (is_parameter_set) {
      for (std::size_t j = ind + 1; j < parser_string.length(); ++j) {
        value_parameter += parser_string[j];
      }
      for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
        if (string_arguments_[i].long_parameter_ == parameter) {
          string_arguments_[i].value_ = value_parameter;
          if (string_arguments_[i].is_valueptr_set_) {
            *string_arguments_[i].valueptr_ = value_parameter;
          }
          if (string_arguments_[i].is_multi_value_) {
            string_arguments_[i].values_.push_back(string_arguments_[i].value_.value());
            if (string_arguments_[i].is_valuesptr_set_) {
              string_arguments_[i].valuesptr_->push_back(string_arguments_[i].value_.value());
            }
          }
          return true;
        }
      }
      for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
        if (int_arguments_[i].long_parameter_ == parameter) {
          try {
            std::size_t len = value_parameter.length();
            std::size_t* lenptr = &len;
            int_arguments_[i].value_ = std::stoi(value_parameter, lenptr);
            if (*lenptr != value_parameter.length()) {
              std::cerr << "ERROR:\nThere should be an integer argument.\n";
              return false;
            }
            if (int_arguments_[i].is_valueptr_set_) {
              *int_arguments_[i].valueptr_ = int_arguments_[i].value_.value();
            }
            if (int_arguments_[i].is_multi_value_) {
              int_arguments_[i].values_.push_back(int_arguments_[i].value_.value());
              if (int_arguments_[i].is_valuesptr_set_) {
                int_arguments_[i].valuesptr_->push_back(int_arguments_[i].value_.value());
              }
            }
            return true;
          } catch (const std::invalid_argument&) {
            std::cerr << "ERROR:\nThere should be an integer argument.\n";
            return false;
          } catch (const std::out_of_range&) {
            std::cerr << "ERROR:\nOut of range in the integer argument.\n";
            return false;
          }
        }
      }
    }
  }
  std::cerr << "ERROR:\nString/int argument has not found.\n";

  return false;
}

bool ArgParser::CheckLongFlag(const std::string_view& flag) {
  bool is_flag_found = false;
  std::cout << "FLAG::   " << flag << '\n';
  for (std::size_t i = 0; i < bool_arguments_.size(); ++i) {
    if (bool_arguments_[i].long_parameter_ == flag) {
      bool_arguments_[i].value_ = true;
      if (bool_arguments_[i].is_valueptr_set_) {
        *bool_arguments_[i].valueptr_ = &bool_arguments_[i].value_;
      }
      return true;
    }
  }
  std::cerr << "ERROR:\nThis flag has not found.\n";

  return false;
}

bool ArgParser::CheckStringPositionalArgument(StringArguments* positional_argument, const std::string& parser_string) {
  if (!positional_argument->is_multi_value_) {
    return false;
  }
  positional_argument->values_.push_back(parser_string);
  if (positional_argument->is_valuesptr_set_) {
    positional_argument->valuesptr_->push_back(parser_string);
  }

  return true;
}

bool ArgParser::CheckIntPositionalArgument(IntArguments* positional_argument, const std::string& parser_string) {
  if (!positional_argument->is_multi_value_) {
    return false;
  }
  try {
    size_t len = parser_string.length();
    size_t* lenptr = &len;
    positional_argument->values_.push_back(std::stoi(parser_string, lenptr));
    if (*lenptr != parser_string.length()) {
      std::cerr << "ERROR\nThere should be an integer positional argument.\n";
      return false;
    }
    if (positional_argument->is_valuesptr_set_) {
      positional_argument->valuesptr_->push_back(positional_argument->values_.back());
    }
    return true;
  } catch (const std::invalid_argument&) {
    std::cerr << "ERROR:\nThere should be an integer positional argument.\n";
    return false;
  } catch (const std::out_of_range&) {
    std::cerr << "ERROR:\nOut of range in the integer positional argument.\n";
    return false;
  }
}

void ArgParser::SetStringDefaultValue(StringArguments* string_argument) {
  for (std::size_t i = string_argument->values_.size(); i < string_argument->min_argc_count_; ++i) {
    string_argument->values_.push_back(string_argument->default_value_.value());
    if (string_argument->is_valuesptr_set_) {
      string_argument->valuesptr_->push_back(string_argument->default_value_.value());
    }
  }
}
void ArgParser::SetIntDefaultValue(IntArguments* int_argument) {
  for (std::size_t i = int_argument->values_.size(); i < int_argument->min_argc_count_; ++i) {
    int_argument->values_.push_back(int_argument->default_value_.value());
    if (int_argument->is_valuesptr_set_) {
      int_argument->valuesptr_->push_back(int_argument->default_value_.value());
    }
  }
}

bool ArgParser::SetDefaultValue() {
  for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
    if (string_arguments_[i].values_.size() < string_arguments_[i].min_argc_count_
        && string_arguments_[i].is_multi_value_) {
      if (!string_arguments_[i].default_value_.has_value()) {
        std::cerr << "ERROR:\nThere is no default value for unentered values.\n";
        return false;
      }
      SetStringDefaultValue(&string_arguments_[i]);
    }
  }
  for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
    if (int_arguments_[i].values_.size() < int_arguments_[i].min_argc_count_
        && int_arguments_[i].is_multi_value_) {
      if (!int_arguments_[i].default_value_.has_value()) {
        std::cerr << "ERROR:\nThere is no default value for unentered values.\n";
        return false;
      }
      SetIntDefaultValue(&int_arguments_[i]);
    }
  }

  return true;
}

bool ArgParser::Parse(const std::vector<std::string>& parser_string) {
  StringArguments* string_positional_argument = nullptr;
  IntArguments* int_positional_argument = nullptr;
  for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
    if (string_arguments_[i].is_positional_) {
      string_positional_argument = &string_arguments_[i];
      break;
    }
  }
  for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
    if (int_arguments_[i].is_positional_) {
      int_positional_argument = &int_arguments_[i];
      break;
    }
  }
  for (std::size_t i = 1; i < parser_string.size(); ++i) {
    if (parser_string[i][0] == '-') {
      if (parser_string[i][1] != '-' && parser_string[i].find('=') != std::string::npos) {
        if (!CheckShortArgument(parser_string[i])) {
          return false;
        }
      } else if (parser_string[i][1] != '-' ) {
        if (parser_string[i][1] >= '0' && parser_string[i][1] <= '9' && int_positional_argument != nullptr) {
          if (!CheckIntPositionalArgument(int_positional_argument, parser_string[i])) {
            return false;
          }
          continue;
        }
        if (!CheckShortFlags(parser_string[i])) {
          return false;
        }
      } else if (parser_string[i][1] == '-' && parser_string[i].find('=') != std::string::npos) {
        if (!CheckLongArgument(parser_string[i])) {
          return false;
        }
      } else if (parser_string[i][1] == '-') {
        std::string_view parser_string_copy = parser_string[i];
        std::string_view long_parameter  = parser_string_copy.substr(2, std::string_view::npos);
        if (long_parameter == help_parser_.long_argument_help_) {
          is_help_set_ = true;
          return true;
        }
        if (!CheckLongFlag(long_parameter)) {
          return false;
        }
      }
    } else {
      std::cout << parser_string[i] << " argvvvvv\n";
      if (string_positional_argument != nullptr) {
        if (!CheckStringPositionalArgument(string_positional_argument, parser_string[i])) {
          return false;
        }
      } else if (int_positional_argument != nullptr) {
        if (!CheckIntPositionalArgument(int_positional_argument, parser_string[i])) {
          return false;
        }
      } else {
        std::cerr << "ERROR\nIncorrect input for positional arguments\n";
        return false;
      }
    }
  }

  if (is_help_set_) {
    return true;
  }
  if (!SetDefaultValue()) {
    return false;
  }

  return IsArgumentsCorrect();
}

bool ArgParser::Parse(std::size_t argc, char* argv[]) {
  for (std::size_t i = 0; i < argc; ++i) {
    parser_string_.push_back(std::string(argv[i]));
  }

  return Parse(parser_string_);
}

std::string ArgParser::GetStringValue(const std::string str_parameter) const {
  for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
    if (string_arguments_[i].long_parameter_ == str_parameter && string_arguments_[i].value_.has_value()) {
      return string_arguments_[i].value_.value();
    }
  }
  std::cerr << "ERROR:\nThis string parameter has not found\n";
  std::exit(EXIT_FAILURE);
}

std::string ArgParser::GetStringValue(const std::string str_parameter, std::size_t index_argument) const {
  bool is_parameter_set = false;
  int32_t index_parameter = 0;
  for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
    if (!string_arguments_[i].is_multi_value_ && string_arguments_[i].long_parameter_ == str_parameter) {
      is_parameter_set = true;
      index_parameter = i;
      break;
    }
  }
  if (!is_parameter_set) {
    std::cerr << "ERROR:\nIncorrect \'GetStringValue\' command.\nParameter " << str_parameter << " doesn't exist ot there is no multi_value.\n";
    std::exit(EXIT_FAILURE);
  }
  if (string_arguments_[index_parameter].values_.size() - 1 <= index_argument) {
    std::cerr << "ERROR:\nIncorrect \'GetStringValue\' command.\nOut of range of the index of the argument\n";
    std::exit(EXIT_FAILURE);
  }

  return string_arguments_[index_parameter].values_[index_argument];
}

bool ArgParser::GetFlag(const std::string str_parameter) const {
  for (std::size_t i = 0; i < bool_arguments_.size(); ++i) {
    if (bool_arguments_[i].long_parameter_ == str_parameter) {
      return bool_arguments_[i].value_;
    }
  }
  std::cerr << "ERROR:\nThis flag has not found\n";
  std::exit(EXIT_FAILURE);
}

int64_t ArgParser::GetIntValue(const std::string str_parameter) const {
  for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
    if (int_arguments_[i].long_parameter_ == str_parameter && int_arguments_[i].value_.has_value()) {
      return int_arguments_[i].value_.value();
    }
  }
  std::cerr << "ERROR:\nThis int parameter has not found\n";
  std::exit(EXIT_FAILURE);
}

int64_t ArgParser::GetIntValue(const std::string str_parameter, std::size_t index_argument) const {
  bool is_parameter_set = false;
  int32_t index_parameter = 0;
  for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
    if (int_arguments_[i].is_multi_value_ && int_arguments_[i].long_parameter_ == str_parameter) {
      is_parameter_set = true;
      index_parameter = i;
      break;
    }
  }
  if (!is_parameter_set) {
    std::cerr << "ERROR:\nIncorrect \'GetIntValue\' command.\nParameter " << str_parameter << " doesn't exist ot there is no multi_value.\n";
    std::exit(EXIT_FAILURE);
  }
  if (int_arguments_[index_parameter].values_.size() - 1 < index_argument) {
    std::cerr << "ERROR:\nIncorrect \'GetIntValue\' command.\nOut of range of the index of the argument\n";
    std::exit(EXIT_FAILURE);
  }

  return int_arguments_[index_parameter].values_[index_argument];
}

StringArguments& ArgParser::AddStringArgument(const char short_parameter, const std::string& long_parameter) {
  string_arguments_.push_back(StringArguments(short_parameter, long_parameter, ""));

  return string_arguments_.back();
}

StringArguments& ArgParser::AddStringArgument(const char short_parameter,
                                              const std::string& long_parameter,
                                              const std::string& description) {
  string_arguments_.push_back(StringArguments(short_parameter, long_parameter, description));

  return string_arguments_.back();
}

StringArguments& ArgParser::AddStringArgument(const std::string& long_parameter, const std::string& description) {
  string_arguments_.push_back(StringArguments('\0', long_parameter, description));

  return string_arguments_.back();
}

StringArguments& ArgParser::AddStringArgument(const char short_parameter) {
  string_arguments_.push_back(StringArguments(short_parameter, "", ""));

  return string_arguments_.back();
}

StringArguments& ArgParser::AddStringArgument(const std::string& long_parameter) {
  string_arguments_.push_back(StringArguments('\0', long_parameter, ""));

  return string_arguments_.back();
}

IntArguments& ArgParser::AddIntArgument(const std::string& long_parameter) {
  int_arguments_.push_back(IntArguments('\0', long_parameter, ""));

  return int_arguments_.back();
}

IntArguments& ArgParser::AddIntArgument(const char short_parameter, const std::string& long_parameter) {
  int_arguments_.push_back(IntArguments(short_parameter, long_parameter, ""));

  return int_arguments_.back();
}

IntArguments& ArgParser::AddIntArgument(const char short_parameter,
                                        const std::string& long_parameter,
                                        const std::string& description) {
  int_arguments_.push_back(IntArguments(short_parameter, long_parameter, description));

  return int_arguments_.back();
}

IntArguments& ArgParser::AddIntArgument(const std::string& long_parameter, const std::string& description) {
  int_arguments_.push_back(IntArguments('\0', long_parameter, description));

  return int_arguments_.back();
}

IntArguments& ArgParser::AddIntArgument(const char short_parameter) {
  int_arguments_.push_back(IntArguments(short_parameter, "", ""));

  return int_arguments_.back();
}

Flags& ArgParser::AddFlag(const std::string& long_parameter) {
  bool_arguments_.push_back(Flags('\0', long_parameter, ""));

  return bool_arguments_.back();
}

Flags& ArgParser::AddFlag(const char short_parameter, const std::string& long_parameter) {
  bool_arguments_.push_back(Flags(short_parameter, long_parameter, ""));

  return bool_arguments_.back();
}

Flags& ArgParser::AddFlag(const char short_parameter,
                          const std::string& long_parameter,
                          const std::string& description) {
  bool_arguments_.push_back(Flags(short_parameter, long_parameter, description));

  return bool_arguments_.back();
}

Flags& ArgParser::AddFlag(const char short_parameter) {
  bool_arguments_.push_back(Flags(short_parameter, "", ""));

  return bool_arguments_.back();
}

Flags& ArgParser::AddFlag(const std::string& long_parameter, const std::string& description) {
  bool_arguments_.push_back(Flags('\0', long_parameter, description));

  return bool_arguments_.back();
}

bool ArgParser::IsStrArgSet(const std::string& str_parameter) const {
  for (std::size_t i = 0; i < string_arguments_.size(); ++i) {
    if (string_arguments_[i].long_parameter_ == str_parameter && string_arguments_[i].value_.has_value()) {
      return true;
    }
  }

  return false;
}

bool ArgParser::IsIntArgSet(const std::string& int_parameter) const {
  for (std::size_t i = 0; i < int_arguments_.size(); ++i) {
    if (int_arguments_[i].long_parameter_ == int_parameter && int_arguments_[i].value_.has_value()) {
      return true;
    }
  }

  return false;
}

void ArgParser::SetUpParser() {
  this->AddHelp('h', "help", "Help of programm");
  this->AddStringArgument('c', "create", "Creates a game").Default("standard");
  this->AddStringArgument('l', "language", "Sets a language. The Possible languages are English, Russian, Hebrew").Default("en");
}

void ArgParser::DisplayError() {
  std::cerr << "ERROR:\nIncorrect arguments.\n" << "\n";
  std::cerr << this->HelpDescription() << "\n";
}

}
