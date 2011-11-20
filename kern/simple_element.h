#pragma once
#include "element.h"

class SimpleElement: public Element {
  virtual bool check(std::vector< Element* >& vars) {
    return false;
  }
  virtual bool equals(Element* ele2) {
    SimpleElement* ele2ex = ele2->cast< SimpleElement > ();
    if (ele2ex) {
      bool result = typeid(*ele2ex) == typeid(*this);
      ele2ex->remove();
      return result;
    }
    else
      return 0;
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
  virtual Element* applyRecursive() {
    return copy();
  }
  virtual Element *_compare(Element*& _ele, std::vector< std::pair< Unknown*, Element* > > &unknwons) {
    return equals(_ele) ? copy() : 0;
  }
};