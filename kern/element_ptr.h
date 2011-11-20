#pragma once

#include "element.h"

class ElementPtr {
private:
  Element *element;
public:
  ElementPtr() : element(0) { }
  ElementPtr (Element *ele) : element (ele) { 
  }
  
  ElementPtr (const ElementPtr &ptr) : element (ptr.element ? ptr.element->copy() : 0) {
  }
  
  ElementPtr (ElementPtr &ptr) : element (ptr.element ? ptr.element->copy() : 0) {
  }
  
  Element *operator -> () {
    return element;
  }
  Element &operator * () {
    return *element;
  }
  
  ElementPtr &operator = (const ElementPtr &ptr2) {
    if (element) element->remove();
    element = ptr2.element;
    if (element) element->copy();
    return *this;
  }
  
  ~ElementPtr() {
    if (element) element->remove();
  }
  ElementPtr type() {
    return ElementPtr (element->type());
  }
};

namespace Creater{
  
ElementPtr variable (int index, ElementPtr type);
ElementPtr function (ElementPtr var, ElementPtr f);
ElementPtr forAll (ElementPtr var, ElementPtr f);
ElementPtr application (ElementPtr f, ElementPtr var);
ElementPtr set ();
ElementPtr prop ();
ElementPtr unknown();
template< class Str >
ElementPtr namedVariable (const Str& string);
template< class Str >
ElementPtr namedForAll (const Str &string, ElementPtr var, ElementPtr f);
template< class Str >
ElementPtr namedFunction (const Str &string, ElementPtr var, ElementPtr f);

void print (ElementPtr e);

};