#include "syntax.h"
#include <iostream>

using namespace Creater;

int main() {
  Element* t = set();
  print(t);
  
  Element *f = forAll (variable (0, t), variable (1, t));
  Element *f2 = forAll (t, f);
  print (f2);
  
  Element *funq = function (prop(), 
		   function (variable(0, prop()), variable(1, prop())));
  print (funq);
  
  Element *fun = function (t,
                  function (variable (0, t),
	           function (variable (1, t),
		    variable(0, variable(2, t->type())))));
  print(fun);
}