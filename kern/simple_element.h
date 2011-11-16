#pragma once
#include "element.h"

class SimpleElement: public Element {
  virtual bool check(std::vector< Element* >& vars) {
    return false;
  }
  virtual bool equals(Element* ele2) {
    return typeid(*ele2) == typeid(*this);
  }
  virtual bool isUsed(int index) {
    return false;
  }
  virtual Element* replace(Element* with, int varId) {
    return copy();
  }
  virtual Element* step(int from) {
    return copy();
  } 
  virtual Element* replaceNamed(Element* with, int T1, void* T2) {
    return copy();
  }
};