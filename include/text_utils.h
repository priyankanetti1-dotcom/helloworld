#ifndef HELLOWORLD_TEXT_UTILS_H
#define HELLOWORLD_TEXT_UTILS_H

#include <cstddef>
#include <string>
#include <vector>

namespace helloworld {

// Returns a newly malloc'd upper-cased copy of `input`.
// Caller owns the returned buffer and must free() it.
char* duplicateUpper(const char* input);

// Length of the upper-cased form of `input`.
std::size_t upperLength(const char* input);

// Returns the domain part of an e-mail address ("a@b.com" -> "b.com").
std::string extractDomain(const std::string& email);

// Renders `text` inside an ASCII banner.
std::string renderBanner(const char* text);

// "one two three" -> "three two one"
std::string reverseWords(const std::string& sentence);

std::vector<std::string> splitWords(const std::string& sentence);

}  // namespace helloworld

#endif  // HELLOWORLD_TEXT_UTILS_H
