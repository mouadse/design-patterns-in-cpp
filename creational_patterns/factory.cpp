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

class Creator {
public:
  virtual ~Creator() {};
  virtual Product *FactoryMethod() const = 0;
  std::string someOperation() const {
    Product *product = this->FactoryMethod();

    std::string result =
        "Creator: The same creator's code has just worked with " +
        product->Operation();
    delete product;
    return result;
  }
};

int main() {
  //
  return 0;
}
