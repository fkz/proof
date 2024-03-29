#pragma once
#include "element_ptr.h"
#include "ext/named_var.h"

namespace Creater {

template< class Str >
ElementPtr namedVariable (const Str &string) {
  return (new NamedVar<Str> (string))->copy();
}

template< class Str >
ElementPtr namedFunction (const Str &string, ElementPtr var, ElementPtr f) {
  ElementPtr f1 = f->step(0);
  Element *ecopy = variable(0, var)->copy();
  ElementPtr f3 = f1->replaceNamed (ecopy, 5, const_cast< Str * > (&string));
  ecopy->remove();
  return function(var, f3);
}

template< class Str >
ElementPtr namedForAll (const Str &string, ElementPtr var, ElementPtr f) {
  ElementPtr f1 = f->step(0);
  Element* vc = variable (0, var)->copy();
  ElementPtr f2 = f1->replaceNamed (vc, 5, const_cast< Str * > (&string));
  vc->remove();
  return forAll(var, f2);
}

};