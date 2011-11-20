#include "abstraction.h"
#include "element_cast.h"
#include "ext/named_var.h"
#include <vector>

Element* Abstraction::step(int from)
{
  Element *newTerm = term->step(from+1);
  Element *newVar = var->step(from);
  if (newTerm == term && newVar == var) {
    newTerm->remove();
    newVar->remove();
    return copy();
  }
  else {
    Element* result = _clone(newTerm, newVar)->copy();
    newTerm->remove();
    newVar->remove();
    return result;
  }
}

Element* Abstraction::replace(Element* with, int varId)
{
  Element* stp = with->step(0);
  Element *newTerm = term->replace(stp, varId+1);
  stp->remove();
  Element *newVar = var->replace(with, varId);
  if (newTerm == term && newVar == var) {
    newTerm->remove();
    newVar->remove();
    return copy();
  }
  else {
    Element* result = _clone(newTerm, newVar)->copy();
    newTerm->remove();
    newVar->remove();
    return result;
  }
}

Element* Abstraction::replaceNamed(Element* with, int T1, void* T2)
{
  Element* stp = with->step(0);
  Element *newTerm = term->replaceNamed(stp, T1, T2);
  stp->remove();
  Element *newVar = var->replaceNamed(with, T1, T2);
  if (newTerm == term && newVar == var) {
    newTerm->remove();
    newVar->remove();
    return copy();
  }
  else {
    Element* result = _clone(newTerm, newVar)->copy();
    newTerm->remove();
    newVar->remove();
    return result;
  }
}


Element* Abstraction::applyRecursive()
{
  Element *newTerm = term->applyRecursive();
  Element *newVar = var->applyRecursive();
  if (newTerm == term && newVar == var) {
    newTerm->remove();
    newVar->remove();
    return copy();
  }
  else {
    Element* result = _clone(newTerm, newVar)->copy();
    newTerm->remove();
    newVar->remove();
    return result;
  }
}



Element* Abstraction::clone()
{
  return _clone (term, var);
}


Abstraction::~Abstraction()
{
  term->remove();
  var->remove();
}


bool Abstraction::equals(Element* ele2) 
{
  Abstraction *e = ele2->cast< Abstraction > ();  
  if (!e)  return false;
  bool result = term->equals(e->term) && var->equals(e->var) && typeid(e) == typeid(this);
  e->remove();
  return result;
}


struct Error {
public:
  bool operator == (const Error &err2) { return false; }  
};

std::ostream &operator << (std::ostream &stream, const Error &) { stream << "ERROR_PLEASE_REPORT"; return stream; }



Element *Abstraction::_compare(Element*& _ele, std::vector< std::pair< Unknown*, Element* > >& unknwons)
{
  Abstraction *e = _ele->cast< Abstraction > ();
  if (!e)
    return 0;
  
  _ele->remove();
  
  Abstraction *_ele_clone;
  _ele = (_ele_clone = _clone(e->term, e->var))->copy();
  e->remove();
  
  Element* var_copy = var->compare(_ele_clone->var, unknwons);
  if (!var_copy) {
    return 0;
  }
  
  for (std::vector< std::pair< Unknown*, Element* > >::iterator it = unknwons.begin(); it != unknwons.end(); ++it) {
    Element* t = it->second->step(0);
    it->second->remove();
    it->second = t;
  }
  Element *term_copy = term->compare(_ele_clone->term, unknwons);
  if (!term_copy)
    return 0;
  Element *error = new NamedVar<Error>(Error());
  error->copy();
  for (std::vector< std::pair< Unknown*, Element* > >::iterator it = unknwons.begin(); it != unknwons.end(); ++it) {
    Element* t = it->second->replace(error, 0);
    it->second->remove();;
    it->second = t;
  }
  
  error->remove();
  Abstraction* result = _clone(term_copy, var_copy);
  term_copy->remove();
  var_copy->remove();
  return result->copy();
}


bool Abstraction::check(std::vector< Element* >& vars)
{
  if (!var->check(vars)) return false;
  std::vector< Element * > vars2;
  for (std::vector< Element* >::const_iterator it = vars.begin(); it != vars.end(); ++it) {
    vars2.push_back ((*it)->step(0));
  }
  vars2.push_back(var->step(0));
  bool result = term->check(vars2);
  for (std::vector< Element* >::const_iterator it = vars2.begin(); it != vars2.end(); ++it) {
    (*it)->remove();
  }
  return result;
}


Abstraction* ForAll::_clone(Element* newTerm, Element* newVar)
{
  return new ForAll(newVar->copy(), newTerm->copy());
}

Element* ForAll::type()
{
  Element* t = getTerm();
  Element* result = t->type();
  t->remove();
  return result;
}

Abstraction* Function::_clone(Element* newTerm, Element* newVar)
{
  return new Function(newVar->copy(), newTerm->copy());
}

Element* Function::type()
{
  Element* t = getTerm();
  ForAll *result = new ForAll (getVar(), t->type());
  t->remove();
  return result->copy();
}
