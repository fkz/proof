#pragma once

#include "syntax.h"

class ElementPtr {
private:
  Element *element;
public:
  ElementPtr (Element *ele) : element (ele) { 
  }
  
  ElementPtr (const ElementPtr &ptr) : element (ptr.element->copy()) {
  }
  
  ElementPtr (ElementPtr &ptr) : element (ptr.element->copy()) {
  }
  
  Element *operator -> () {
    return element;
  }
  Element *operator * () {
    return element;
  }
  
  ~ElementPtr() {
    element->remove();
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

void print (ElementPtr e);

};