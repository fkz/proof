#pragma once
#include "element_ptr.h"
#include "ext/named_var.h"

namespace Creater {

template< class Str >
ElementPtr namedVariable (const Str &string) {
  return (new NamedVar<Str> (string))->clone();
}

template< class Str >
ElementPtr namedFunction (const Str &string, ElementPtr var, ElementPtr f) {
  f = f->step(0);
  f = f->replaceNamed (variable(0, var)->copy(), 5, &string);
  return function(var, f);
}

template< class Str >
ElementPtr namedForAll (const Str &string, ElementPtr var, ElementPtr f) {
  f = f->step(0);
  f = f->replaceNamed (variable (0, var)->copy(), 5, const_cast< Str * > (&string));
  return forAll(var, f);
}

};