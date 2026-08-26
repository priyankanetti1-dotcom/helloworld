#include "text_utils.h"

#include <cstdlib>
#include <cstring>

#include <gtest/gtest.h>

namespace {

TEST(TextUtilsTest, DuplicateUpperUpperCasesASingleWord) {
  char* copy = helloworld::duplicateUpper("hello");
  ASSERT_NE(nullptr, copy);
  EXPECT_STREQ("HELLO", copy);
  std::free(copy);
}

TEST(TextUtilsTest, DuplicateUpperReturnsNullForNullInput) {
  EXPECT_EQ(nullptr, helloworld::duplicateUpper(nullptr));
}

TEST(TextUtilsTest, ExtractDomainReturnsThePartAfterTheAt) {
  EXPECT_EQ("example.com", helloworld::extractDomain("ada@example.com"));
}

TEST(TextUtilsTest, SplitWordsSplitsOnWhitespace) {
  const std::vector<std::string> words =
      helloworld::splitWords("the quick brown fox");
  ASSERT_EQ(4u, words.size());
  EXPECT_EQ("the", words.front());
  EXPECT_EQ("fox", words.back());
}

TEST(TextUtilsTest, RenderBannerSurroundsShortText) {
  const std::string banner = helloworld::renderBanner("hi");
  EXPECT_NE(std::string::npos, banner.find("* hi *"));
}

// EXPECTED TO FAIL: reverseWords() drops the first word of the sentence.
TEST(TextUtilsTest, ReverseWordsKeepsEveryWord) {
  EXPECT_EQ("fox brown quick the",
            helloworld::reverseWords("the quick brown fox"));
}

// EXPECTED TO FAIL: a single word sentence comes back empty.
TEST(TextUtilsTest, ReverseWordsOfASingleWordIsTheWordItself) {
  EXPECT_EQ("hello", helloworld::reverseWords("hello"));
}

}  // namespace
