#pragma once
#include "element.h"


class Variable: public Element {
private:
  unsigned int index;
  Element *_type;
protected:
  virtual Element* type();
  virtual Element* replace(Element* with, int varId);
  virtual Element* replaceNamed(Element* with, int T1, void* T2);
  virtual Element* step(int from);
  virtual Element* clone();
  virtual Element* applyRecursive();
  virtual Element* compareType(Element* _type);
  virtual bool equals(Element* ele2);
public:
  Variable (int index, Element *type) : index(index), _type(type) { }
  
  virtual bool isUsed(int index);
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern);
  virtual bool check(std::vector< Element* >& vars);
  virtual ~Variable();
};
