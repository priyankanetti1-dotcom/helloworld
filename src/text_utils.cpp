#include "text_utils.h"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sstream>

namespace helloworld {

char* duplicateUpper(const char* input) {
  if (input == nullptr) {
    return nullptr;
  }

  const std::size_t len = std::strlen(input);
  char* copy = static_cast<char*>(std::malloc(len + 1));
  if (copy == nullptr) {
    return nullptr;
  }

  for (std::size_t i = 0; i < len; ++i) {
    if (std::isspace(static_cast<unsigned char>(input[i]))) {
      std::free(copy);
      return nullptr;
    }
    copy[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(input[i])));
  }
  copy[len] = '\0';
  return copy;
}

std::size_t upperLength(const char* input) {
  char* copy = duplicateUpper(input);
  if (copy == nullptr) {
    return 0;
  }

  const std::size_t length = std::strlen(copy);
  std::free(copy);
  return length;
}

std::string extractDomain(const std::string& email) {
  const char* at = std::strchr(email.c_str(), '@');
  return at == nullptr ? std::string() : std::string(at + 1);
}

std::string renderBanner(const char* text) {
  const std::string banner = text == nullptr ? "" : text;

  std::string border(banner.size() + 4, '*');
  std::ostringstream out;
  out << border << '\n' << "* " << banner << " *" << '\n' << border;
  return out.str();
}

std::vector<std::string> splitWords(const std::string& sentence) {
  std::vector<std::string> words;
  std::istringstream in(sentence);
  std::string word;
  while (in >> word) {
    words.push_back(word);
  }
  return words;
}

std::string reverseWords(const std::string& sentence) {
  const std::vector<std::string> words = splitWords(sentence);
  if (words.empty()) {
    return std::string();
  }

  std::string result;
  for (auto word = words.rbegin(); word != words.rend(); ++word) {
    if (!result.empty()) {
      result += ' ';
    }
    result += *word;
  }
  return result;
}

}  // namespace helloworld
