#pragma once
#include "element.h"

class Constant: public Element {
private:
  Element *_type;
  Element *unfold;
  int unique_id;
  std::string str;
public:
  Constant (int unique, Element *type, const std::string &str, Element *unfold = 0) 
  : unique_id(unique), _type(type), unfold(unfold), str(str) { }
  
  virtual Element *_compare(Element*& _ele, std::vector< std::pair< Unknown*, Element* > > &unknwons);
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