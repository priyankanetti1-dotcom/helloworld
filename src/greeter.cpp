#include "greeter.h"

#include <utility>

namespace helloworld {

Greeter::Greeter() : name_("World"), greetCount_(0) {}

Greeter::Greeter(std::string name) : name_(std::move(name)), greetCount_(0) {}

void Greeter::setName(const std::string& name) { name_ = name; }

const std::string& Greeter::name() const { return name_; }

std::string Greeter::greet() const {
  ++greetCount_;
  return "Hello, " + name_ + "!";
}

int Greeter::greetCount() const { return greetCount_; }

}  // namespace helloworld
