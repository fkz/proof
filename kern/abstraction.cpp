#include "abstraction.h"

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
    return _clone(newTerm, newVar);
  }
}

Element* Abstraction::replace(Element* with, int varId)
{
  Element *newTerm = term->replace(with->step(0), varId+1);
  Element *newVar = var->replace(with, varId);
  if (newTerm == term && newVar == var) {
    newTerm->remove();
    newVar->remove();
    return copy();
  }
  else {
    return _clone(newTerm, newVar);
  }
}

Element* Abstraction::replaceNamed(Element* with, int T1, void* T2)
{
  Element *newTerm = term->replaceNamed(with->step(0), T1, T2);
  Element *newVar = var->replaceNamed(with, T1, T2);
  if (newTerm == term && newVar == var) {
    newTerm->remove();
    newVar->remove();
    return copy();
  }
  else {
    return _clone(newTerm, newVar);
  }
}


Element* Abstraction::clone()
{
  return _clone (term->copy(), var->copy());
}


Abstraction::~Abstraction()
{
  term->remove();
  var->remove();
}


bool Abstraction::equals(Element* ele2) 
{
  Abstraction *e = dynamic_cast< Abstraction * > (ele2);  
  if (!e) { return ele2->equals_ex(this); }
  return term->equals(e->term) && var->equals(e->var) && typeid(e) == typeid(this);
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
  return new Function(newVar->clone(), newTerm->clone());
}

Element* Function::type()
{
  Element* t = getTerm();
  ForAll *result = new ForAll (getVar(), t->type());
  t->remove();
  return result->copy();
}
