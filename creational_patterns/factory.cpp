#include <iostream>
#include <string>

using namespace std;

class Product {
public:
  virtual ~Product() {}
  virtual std::string Operation() const = 0;
};

class ConcreteProduct1 : public Product {
public:
  std::string Operation() const override {
    return "Results of the ConcreteProduct1";
  }
};

class ConcreteProduct2 : public Product {
public:
  std::string Operation() const override {
    return "Results of the ConcreteProduct2";
  }
};

int main() {
  //
  return 0;
}
