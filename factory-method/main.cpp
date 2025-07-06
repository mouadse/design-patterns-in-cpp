#include <iostream>
#include <string>

class Product {
public:
  virtual ~Product() {}
  virtual std::string Operation() const = 0;
};

class ConcreteProduct1 : public Product {
public:
  std::string Operation() const override {
    return "{Result of ConcreteProduct1}";
  }
};

class ConcreteProduct2 : public Product {
public:
  std::string Operation() const override {
    return "{Result of ConcreteProduct2}";
  }
};

class Creator {
public:
  virtual ~Creator() {}
  virtual Product *FactoryMethod() const = 0;
  std::string SomeOperation() {
    Product *prd = this->FactoryMethod();
    std::string result =
        "Creator: The same creator's code has just worked with " +
        prd->Operation();
    delete prd;
    return result;
  }
};

int main() {
  // Code demo goes here
  return 0;
}
