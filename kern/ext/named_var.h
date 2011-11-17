#pragma once
#include "../simple_element.h"
#include "unknown.h"
#include "../element_cast.h"

template< class Str >
class NamedVar: public SimpleElement {
private:
  Str str;
public:
  NamedVar (const Str &str): str(str) { }
  
  virtual Element* clone() {
    return new NamedVar (str);
  }
  
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) {
    stream << "NAMED<" << str << ">";
  }
  
  virtual Element* type() {
    return (new Unknown())->copy();
  }
  
  virtual bool equals(Element* ele2) {
    NamedVar *result = ele2->cast< NamedVar > ();
    if (result) {
      bool res = result->str == str;
      result->remove();
      return res;
    }
    else
      return false;
  }
  virtual Element* replaceNamed(Element* with, int T1, void* T2) {
    if (T1 == 5) {
      if (*static_cast< Str * > (T2) == str)
	return with->copy();
      else
	return copy();
    }
    if (T1 == 10) {
      //...
    }
    return copy();
  }
  Str name() {
    return str;
  }
};