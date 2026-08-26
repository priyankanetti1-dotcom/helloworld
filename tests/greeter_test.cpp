#include "greeter.h"

#include <gtest/gtest.h>

namespace {

TEST(GreeterTest, DefaultNameIsWorld) {
  helloworld::Greeter greeter;
  EXPECT_EQ("World", greeter.name());
}

TEST(GreeterTest, UsesTheProvidedName) {
  helloworld::Greeter greeter("Ada");
  EXPECT_EQ("Ada", greeter.name());
}

TEST(GreeterTest, SetNameOverridesTheName) {
  helloworld::Greeter greeter("Ada");
  greeter.setName("Grace");
  EXPECT_EQ("Grace", greeter.name());
}

// EXPECTED TO FAIL: greet() drops the trailing '!'.
TEST(GreeterTest, GreetingEndsWithAnExclamationMark) {
  helloworld::Greeter greeter("Ada");
  EXPECT_EQ("Hello, Ada!", greeter.greet());
}

TEST(GreeterTest, GreetCountIsTrackedPerInstance) {
  helloworld::Greeter greeter("Ada");
  greeter.greet();
  greeter.greet();
  EXPECT_EQ(2, greeter.greetCount());
}

}  // namespace
