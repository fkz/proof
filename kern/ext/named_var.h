#pragma once
#include "../simple_element.h"
#include "unknown.h"
#include "../element_cast.h"

template< class Str >
class NamedVar: public SimpleElement {
private:
  Str str;
public:
  static const int REPLACE_ID = 5;
  static const int REPLACE_ELEMENT = 15;
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
    if (T1 == REPLACE_ID) {
      if (*static_cast< Str * > (T2) == str)
	return with->copy();
      else
	return copy();
    }
    else if (T1 == REPLACE_ELEMENT) {
      Element *ele = static_cast< Element * > (T2);
      NamedVar<Str>* ele2 = ele->cast< NamedVar<Str> > ();
      if (ele2) {
	bool result = str == ele2->str;
	ele2->remove();
	return result ? with->copy() : copy();
      }
      else
	return copy();
    }
    else
      return copy();
  }
  Str name() {
    return str;
  }
};