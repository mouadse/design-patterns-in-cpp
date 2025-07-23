#include <iostream>
#include <string>

using namespace std;

class Character {
public:
  virtual ~Character() {};
  virtual void attack() = 0;
  virtual void defend() = 0;
};

class Warrior : public Character {
public:
  void attack() override { std::cout << "Warrior attacks with a sword\n"; }
  void defend() override { std::cout << "Warrior defends with a shield\n"; }
};

class Mage : public Character {
public:
  void attack() override { std::cout << "Mage attacks with a spell\n"; }
  void defend() override { std::cout << "Mage defends with a spell\n"; }
};

class CharacterCreator {
public:
  virtual ~CharacterCreator() {};
  virtual Character *createCharcter() = 0;
  void playCharacter() {
    Character *character = createCharcter();
    character->attack();
    character->defend();
    delete character;
  }
};

class WarriorCreator : public CharacterCreator {
public:
  Character *createCharcter() override { return new Warrior(); }
};

int main() { return 0; }
