#pragma once
#include "../simple_element.h"
#include "unknown.h"

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
    NamedVar *result = dynamic_cast< NamedVar * > (ele2);
    if (result)
      return result->str == str;
    else
      return ele2->equals_ex(this);
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