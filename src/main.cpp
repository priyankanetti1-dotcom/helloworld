#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "greeter.h"
#include "stats.h"
#include "text_utils.h"

int main(int argc, char** argv) {
  const std::string name = (argc > 1) ? argv[1] : "World";

  helloworld::Greeter greeter(name);
  std::cout << helloworld::renderBanner("HelloWorld") << '\n';
  std::cout << greeter.greet() << '\n';

  const std::vector<int> samples = {3, 1, 4, 1, 5, 9, 2, 6};
  std::cout << "mean   : " << helloworld::mean(samples) << '\n';
  std::cout << "median : " << helloworld::median(samples) << '\n';
  std::cout << "max    : " << helloworld::maxValue(samples) << '\n';

  std::cout << "reversed: " << helloworld::reverseWords("the quick brown fox")
            << '\n';

  char* shout = helloworld::duplicateUpper(name.c_str());
  if (shout != nullptr) {
    std::cout << "shout  : " << shout << '\n';
    // Coverity RESOURCE_LEAK: `shout` is never freed.
  }

  return EXIT_SUCCESS;
}
