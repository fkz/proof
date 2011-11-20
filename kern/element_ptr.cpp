#include "element_ptr.h"
#include "kern/element_ptr.ext.h"
#include <iostream>
#include "application.h"
#include "abstraction.h"
#include "variable.h"
#include "ext/named_var.h"
#include <sstream>


namespace Creater{
  
ElementPtr variable (int index, ElementPtr type) {
  return (new Variable (index, type->copy()))->copy();
}

ElementPtr function (ElementPtr var, ElementPtr f) {
  return (new Function (var->copy(), f->copy()))->copy();
}

ElementPtr forAll (ElementPtr var, ElementPtr f) {
  return (new ForAll (var->copy(), f->copy()))->copy();
}

ElementPtr application (ElementPtr f, ElementPtr var) {
  return (new Application (f->copy(), var->copy()))->copy();
}


ElementPtr set()
{
  return (new TheTypeOfAllTypesTM())->copy();
}

ElementPtr prop()
{
  return (new Prop())->copy();
}

ElementPtr unknown()
{
  return (new Unknown)->copy();
}




void print (ElementPtr ele) {
  if (!ele->check ()) {
    std::cout << "NOT VALID(!)" << std::endl;
  }
  //else
  {
    std::vector< std::string > strings;
    ele->toString(std::cout, strings, false);
    std::cout << " of type ";
    Element* tt = ele->type();
    tt->toString(std::cout, strings, false);
    tt->remove();
    std::cout << std::endl;
  }
}

std::string printex (ElementPtr ele) {
  std::stringstream stream;
  std::vector< std::string > strings;
  ele->toString (stream, strings, false);
  return stream.str();
}
  
};

std::string p (Element *ele) {
  ElementPtr ppp (ele->copy());
  return Creater::printex(ppp);
}
