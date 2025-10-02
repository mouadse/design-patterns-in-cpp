#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Base Product

class Vehicle {
public:
  virtual void drive() = 0;
  virtual ~Vehicle() = default;
};

// Concrete Products

class Car : public Vehicle {
public:
  void drive() override { std::cout << "Driving a Car." << std::endl; }
};

class Bike : public Vehicle {
public:
  void drive() override { std::cout << "Riding a bike." << std::endl; }
};

// Factory

class VehicleFactory {
public:
  static std::unique_ptr<Vehicle> createVehicle(const std::string &type) {
    if ("car" == type) {
      return std::make_unique<Car>();
    } else if ("bike" == type) {
      return std::make_unique<Bike>();
    }
    return nullptr;
  }
};

int main() {
  auto v1 = VehicleFactory::createVehicle("car");
  auto v2 = VehicleFactory::createVehicle("bike");

  if (v1) {
    v1->drive();
  }

  if (v2) {
    v2->drive();
  }

  return 0;
}
