#pragma once
#include "../simple_element.h"

class Unknown: public SimpleElement {
private:
  int i;
  Unknown *_t;
  static int count;
  Unknown(int i): i (i) { }
public:
  static const int REPLACE_ELEMENT = 25;
  Unknown () : i(count++) { }
  
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) {
    stream << "Unknown" << i;
  }
  virtual Element* type() {
    if (_t)
      return _t->copy();
    else
      return (_t = (Unknown*)(new Unknown())->copy())->copy();
  }
  
  
  virtual bool equals(Element* ele2) {
    Unknown* ele2ex = ele2->cast<Unknown> ();
    bool result = ele2ex && ele2ex->i == i;
    if (ele2ex) ele2ex->remove();
    return result;
  }
  
  virtual Element* replaceNamed(Element* with, int T1, void* T2);
  
  virtual Element *_compare(Element*& _ele, std::vector< std::pair< Unknown*, Element* > > &unknwons) {
    Element* thisC = copy();
    Element* result = _ele->compare(thisC, unknwons);
    _ele->remove();
    _ele = result;
    return thisC;
  }
};