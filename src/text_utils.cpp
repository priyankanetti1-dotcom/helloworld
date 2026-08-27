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
      // Coverity RESOURCE_LEAK: `copy` goes out of scope without being freed.
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

  std::free(copy);
  // Coverity USE_AFTER_FREE: `copy` is read after it has been released.
  return std::strlen(copy);
}

std::string extractDomain(const std::string& email) {
  const char* at = std::strchr(email.c_str(), '@');
  // Coverity NULL_RETURNS / FORWARD_NULL: strchr() may return NULL.
  return std::string(at + 1);
}

std::string renderBanner(const char* text) {
  char banner[32];
  // Coverity OVERRUN / STRING_OVERFLOW: `text` is copied without a bound check.
  std::strcpy(banner, text);

  std::string border(std::strlen(banner) + 4, '*');
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
  for (std::size_t i = words.size(); i-- > 0;) {
    if (!result.empty()) {
      result += ' ';
    }
    result += words[i];
  }
  return result;
}

}  // namespace helloworld
