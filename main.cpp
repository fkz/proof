#include "syntax.h"
#include <iostream>
#include "element.h"

using namespace Creater;

int main() {
  ElementPtr t = set();
  print(t);
  
  ElementPtr f = forAll (variable (0, t), variable (1, t));
  ElementPtr f2 = forAll (t, f);
  print (f2);
  
  
  ElementPtr funq = function (prop(), 
		   function (variable(0, prop()), variable(1, prop())));
  print (funq);
  ElementPtr fun = function (t,
                  function (variable (0, t),
	           function (variable (1, t),
		    variable(0, variable(2, t.type())))));
  print(fun);
}