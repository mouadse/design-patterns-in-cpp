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
  std::string SomeOperation() const {
    Product *prd = this->FactoryMethod();
    std::string result =
        "Creator: The same creator's code has just worked with " +
        prd->Operation();
    delete prd;
    return result;
  }
};

class ConcreteCreator1 : public Creator {
public:
  Product *FactoryMethod() const override {
    // New concrete prod to be created and returned
    return new ConcreteProduct1();
  }
};

class ConcreteCreator2 : public Creator {
public:
  Product *FactoryMethod() const override {
    // New concrete prd will be created and returned
    return new ConcreteProduct2();
  }
};

void ClientCode(const Creator &creator) {
  std::cout
      << "Client: I am not aware of the creator's class, but it still works\n"
      << creator.SomeOperation() << std::endl;
}

int main() {
  std::cout << "App: Launched with the ConcreteCreator1.\n";
  Creator *creator1 = new ConcreteCreator1();
  ClientCode(*creator1);
  std::cout << std::endl;
  Creator *creator2 = new ConcreteCreator2();
  ClientCode(*creator2);
  delete creator1;
  delete creator2;
  return 0;
}
