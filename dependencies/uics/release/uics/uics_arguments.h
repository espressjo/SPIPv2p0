#ifndef ARGPARSER_H
#define ARGPARSER_H
#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class ArgParser {
private:
  std::vector<std::string> args;
  std::unordered_map<std::string, std::string> options;
  std::vector<std::string> positional;

  void parse();

  template <typename T> T cast(const std::string &value) const;

public:
  ArgParser(int argc, char *argv[]);

  // Check if a flag exists
  bool hasFlag(const std::string &flag) const;

  // Get raw string value
  std::string get(const std::string &key,
                  const std::string &default_value = "") const;

  // Get value with automatic type casting
  template <typename T>
  T getAs(const std::string &key, const T &default_value = T()) const;

  // Get positional argument
  std::string getPositional(size_t index,
                            const std::string &default_value = "") const;

  template <typename T>
  T getPositionalAs(size_t index, const T &default_value = T()) const;

  // Get all positional arguments
  const std::vector<std::string> &getPositionalArgs() const;

  // Get number of positional arguments
  size_t positionalCount() const;

  // Get all options
  const std::unordered_map<std::string, std::string> &getOptions() const;
};

// Template implementations must be in header
template <typename T> T ArgParser::cast(const std::string &value) const {
  std::istringstream iss(value);
  T result;
  if (!(iss >> result)) {
    throw std::invalid_argument("Cannot convert '" + value +
                                "' to requested type");
  }
  return result;
}

template <> inline bool ArgParser::cast<bool>(const std::string &value) const {
  std::string lower = value;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

  if (lower == "true" || lower == "1" || lower == "yes" || lower == "on") {
    return true;
  } else if (lower == "false" || lower == "0" || lower == "no" ||
             lower == "off") {
    return false;
  }
  throw std::invalid_argument("Cannot convert '" + value + "' to bool");
}

template <>
inline std::string
ArgParser::cast<std::string>(const std::string &value) const {
  return value;
}

template <typename T>
T ArgParser::getAs(const std::string &key, const T &default_value) const {
  auto it = options.find(key);
  if (it == options.end()) {
    return default_value;
  }
  return cast<T>(it->second);
}

template <typename T>
T ArgParser::getPositionalAs(size_t index, const T &default_value) const {
  if (index >= positional.size()) {
    return default_value;
  }
  return cast<T>(positional[index]);
}

#endif // ARGPARSER_H
