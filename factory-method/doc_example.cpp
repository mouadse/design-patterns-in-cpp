#include <iostream>
#include <string>

class Document {
public:
  virtual ~Document() {}
  virtual void open() = 0;
  virtual void save() = 0;
};

class TextDocument : public Document {
public:
  void open() override { std::cout << "Opening text document\n"; }
  void save() override { std::cout << "Saving text document\n"; }
};

class ImageDocument : public Document {
public:
  void open() override { std::cout << "Opening image document\n"; }
  void save() override { std::cout << "Saving image document\n"; }
};

// Abstract creator class --> The factory

class DocumentCreator {
public:
  virtual Document *createDocument() = 0; // The factory method
  virtual ~DocumentCreator() {}
  void newDocument() {
    Document *doc = createDocument();
    doc->open();
  }
};

class TextDocCreator : public DocumentCreator {
public:
  Document *createDocument() override { return new TextDocument(); }
};

class ImageDocCreator : public DocumentCreator {
public:
  Document *createDocument() override { return new ImageDocument(); }
};

int main() {
  // Code logic here
  return 0;
}
