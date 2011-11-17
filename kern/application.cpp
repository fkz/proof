#include "application.h"
#include "abstraction.h"
#include "element_cast.h"

Element* Application::replace(Element* with, int varId)
{
  Element *newF = f->replace(with, varId);
  Element *newVar = var->replace(with, varId);
  if (newVar == var && newF == f) {
    newVar->remove();
    newF->remove();
    return copy();
  }
  else {
    return (new Application(newF, newVar))->copy();
  }
}

Element* Application::applyRecursive()
{
  Element* newVar = var->applyRecursive();
  Element* newF = f->applyRecursive();
  
  Application *newThis;
  if (newVar == var && newF == f) {
    newVar->remove();
    newF->remove();
    newThis = this;
    copy();
  }
  else {
    newThis = (new Application (newF, newVar));
    newThis->copy();
  }
  
  Element* result = newThis->apply();
  if (result) {
    Element* result2 = result->applyRecursive();
    result->remove();
    newThis->remove();
    return result2;
  }
  else
    return newThis;
}


Element* Application::replaceNamed(Element* with, int T1, void* T2)
{
  Element *newF = f->replaceNamed(with, T1, T2);
  Element *newVar = var->replaceNamed(with, T1, T2);
  if (newVar == var && newF == f) {
    newVar->remove();
    newF->remove();
    return copy();
  }
  else {
    return (new Application(newF, newVar))->copy();
  }
}


Application::~Application()
{
  f->remove();
  var->remove();
}

Element* Application::step(int from)
{
  Element *newF = f->step(from);
  Element *newVar = var->step(from);
  if (newF == f && newVar == var) {
    newF->remove();
    newVar->remove();
    return copy();
  }
  else {
    return (new Application(newF, newVar))->copy();
  }
}

bool Application::check(std::vector< Element* >& vars)
{
  if (!var->check(vars)) return false;
  if (!f->check(vars)) return false;
  Element* t = f->type();
  ForAll* all = t->cast< ForAll >();
  t->remove();
  if (!all)
    return false;
  Element* vtype = all->getVar();
  Element* vartype = var->type();
  bool result = vtype->equals(vartype);
  vtype->remove();
  vartype->remove();
  return result;
}


Element* Application::clone()
{
  return new Application (f->copy(), var->copy());
}

Element* Application::apply()
{
  Function* fun = f->cast< Function >();
  if (!fun) return 0;
  Element* term = fun->getTerm();
  fun->remove();
  Element* result = term->replace(var, 0);
  term->remove();
  return result;
}


bool Application::equals(Element* ele2)
{
  Application *e = dynamic_cast< Application * > (ele2);
  if (!e) { return equals_ex (ele2); }
  bool result = f->equals (e->f) && var->equals(e->var);
  if (result)
    return true;
  else
    return equals_ex(ele2);
}

bool Application::equals_ex(Element* ele2)
{
  Element *simplified = apply();
  if (simplified)
    return simplified->equals(ele2);
  else
    return false;
}



Element* Application::type()
{
  Element *t = f->type();
  ForAll* ft = t->cast< ForAll > (); 
  
  Element* result = ft->getTerm();
  Element* result2 = result->replace(var, 0);
  ft->remove();
  result->remove();
  return result2;
}
