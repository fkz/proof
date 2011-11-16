#pragma once
#include "element.h"

class Application: public Element {
private:
  Element *f;
  Element *var;
protected:
  virtual Element* type();
  virtual Element* replace(Element* with, int varId);
  virtual Element* replaceNamed(Element* with, int T1, void* T2);
  virtual Element* step(int from);
  virtual ~Application();
  virtual Element* clone();
  virtual bool equals(Element* ele2);
  virtual bool equals_ex(Element* ele2);
  virtual bool check(std::vector< Element* >& vars);
public:
  Application (Element *f, Element *var): f (f), var (var) { }
  
  virtual bool isUsed(int index);
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern);
  Element *apply();
};