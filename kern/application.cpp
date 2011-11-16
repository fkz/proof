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
    return new Application(newF, newVar);
  }
}

bool Application::check(std::vector< Element* >& vars)
{
  if (!var->check(vars)) return false;
  if (!f->check(vars)) return false;
  Element* t = f->type();
  ForAll* all = t->cast< ForAll >();
  t->remove();
  Element* v = all->getVar();
  Element* vtype = v->type();
  Element* vartype = var->type();
  bool result = vtype->equals(vartype);
  v->remove();
  vtype->remove();
  vartype->remove();
  return result;
}


Element* Application::clone()
{
  return new Application (f->clone(), var->clone());
}

Element* Application::apply()
{
  return f->replace(var, 0);
}


bool Application::equals(Element* ele2)
{
  Application *e = dynamic_cast< Application * > (ele2);
  if (!e) { return ele2->equals_ex (this); }
  bool result = f->equals (e->f) && var->equals(e->var);
  if (result)
    return true;
  else
    return equals_ex(ele2);
}

bool Application::equals_ex(Element* ele2)
{
  Element *simplified = apply();
  return simplified->equals(ele2);
}



Element* Application::type()
{
  Element *t = f->type();
  ForAll* ft = dynamic_cast< ForAll * > (t); 
  Element* result = ft->getTerm();
  ft->remove();
  return result;
}
