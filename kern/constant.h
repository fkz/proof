#pragma once
#include "element.h"

class Constant: public Element {
private:
  Element *_type;
  Element *unfold;
  int unique_id;
public:
  Constant (int unique, Element *type, Element *unfold = 0) 
  : unique_id(unique), _type(type), unfold(unfold) { }
  
  virtual bool check(std::vector< Element* >& vars);
  virtual Element* applyRecursive();
  virtual Element* clone();
  virtual bool equals(Element* ele2);
  virtual bool isUsed(int index);
  virtual Element* replace(Element* with, int varId);
  virtual Element* replaceNamed(Element* with, int T1, void* T2);
  virtual Element* step(int from);
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern);
  virtual Element* type();
  virtual Element *apply();
};