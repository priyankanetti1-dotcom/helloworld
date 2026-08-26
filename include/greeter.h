#ifndef HELLOWORLD_GREETER_H
#define HELLOWORLD_GREETER_H

#include <string>

namespace helloworld {

class Greeter {
 public:
  Greeter();
  explicit Greeter(std::string name);

  void setName(const std::string& name);
  const std::string& name() const;

  std::string greet() const;

  // Number of times greet() has been called on this instance.
  int greetCount() const;

 private:
  std::string name_;
  mutable int greetCount_;
};

}  // namespace helloworld

#endif  // HELLOWORLD_GREETER_H
