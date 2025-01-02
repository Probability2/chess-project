#pragma once

#include "string_argument_class.hpp"
#include "int_argument_class.hpp"
#include "flag_class.hpp"

#include <iostream>
#include <vector>
#include <optional>
#include <string>
#include <string_view>

namespace ArgumentParser {

struct HelpParser {
  char short_argument_help_ = '\0';
  std::string long_argument_help_;
  std::string description_;
};

class ArgParser {
  public:
    ArgParser(const std::string& name);

    void SetUpParser();

    void DisplayError();

    void AddHelp(const char short_argument, const std::string& long_argument, const std::string& description);
    std::string HelpDescription();
    bool Help() const;

    bool Parse(const std::vector<std::string>& parser_string);
    bool Parse(std::size_t argc, char* argv[]);

    StringArguments& AddStringArgument(const char short_parameter, const std::string& long_parameter);
    StringArguments& AddStringArgument(const char short_parameter);
    StringArguments& AddStringArgument(const std::string& long_parameter);
    StringArguments& AddStringArgument(const char short_parameter, const std::string& long_parameter, const std::string& description);
    StringArguments& AddStringArgument(const std::string& long_parameter, const std::string& description);

    IntArguments& AddIntArgument(const char short_parameter, const std::string& long_parameter);
    IntArguments& AddIntArgument(const char short_parameter);
    IntArguments& AddIntArgument(const std::string& long_parameter);
    IntArguments& AddIntArgument(const char short_parameter, const std::string& long_parameter, const std::string& description);
    IntArguments& AddIntArgument(const std::string& long_parameter, const std::string& description);

    Flags& AddFlag(const char short_parameter, const std::string& long_parameter);
    Flags& AddFlag(const char short_parameter);
    Flags& AddFlag(const std::string& long_parameter);
    Flags& AddFlag(const char short_parameter, const std::string& long_parameter, const std::string& description);
    Flags& AddFlag(const std::string& long_parameter, const std::string& description);

    std::string GetStringValue(const std::string str_parameter) const;
    int64_t GetIntValue(const std::string str_parameter) const;
    bool GetFlag(const std::string str_parameter) const;

    std::string GetStringValue(const std::string str_parameter, std::size_t index_argument) const;
    int64_t GetIntValue(const std::string str_parameter, std::size_t index_argument) const;

    bool IsArgumentsCorrect() const;

    void WriteDescription(const char short_parameter, const std::string& long_parameter, const std::string& description);
    void BuildStringArgumentDescription(StringArguments* string_argument);
    void BuildIntArgumentDescription(IntArguments* int_argument);
    void BuildFlagDescription(Flags* flag);

    bool SetDefaultValue();
    void SetStringDefaultValue(StringArguments* string_argument);
    void SetIntDefaultValue(IntArguments* int_argument);

    bool CheckShortArgument(const std::string_view& parser_string);
    bool CheckLongArgument(const std::string_view& parser_string);
    bool CheckShortFlags(const std::string& parser_string);
    bool CheckLongFlag(const std::string_view& parser_string);
    bool CheckStringPositionalArgument(StringArguments* positional_argument, const std::string& parser_string);
    bool CheckIntPositionalArgument(IntArguments* positional_argument, const std::string& parser_string);

    bool IsStrArgSet(const std::string& str_parameter) const;

    bool IsIntArgSet(const std::string& int_parameter) const;

  private:
    HelpParser help_parser_;
    bool is_help_set_ = false;
    std::string name_parser_;
    std::vector<std::string> parser_string_;
    std::vector<StringArguments> string_arguments_;
    std::vector<IntArguments> int_arguments_;
    std::vector<Flags> bool_arguments_;
    std::string description_parser_;
};
} // namespace ArgumentParser
