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
  //TODO: if's are only used in _compare, maybe code should be there
  if(f)f->remove();
  if(var)var->remove();
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
  all->remove();
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
  if (!e) {
    return equals_ex (ele2); 
  }
  bool result = f->equals (e->f) && var->equals(e->var);
  if (result)
    return true;
  else
    return equals_ex(ele2);
}

Element *Application::_compare(Element*& _ele, std::vector< std::pair< Unknown*, Element* > >& unknwons)
{
  Application *e = _ele->cast< Application > ();
  if (e) {
    Application* _ele_clone = new Application(e->f->copy(), e->var->copy());
    e->remove();
    
    std::vector< std::pair< Unknown*, Element* > > unkn2 = unknwons;
    Application* result = new Application (f->compare (_ele_clone->f, unkn2), var->compare(_ele_clone->f, unkn2));
    result->copy();
    if (result->f && result->var) {
      _ele->remove();
      _ele = _ele_clone->copy();
      return result->copy();
    }
    else {
      result->remove();
      delete _ele_clone;
      Element* simplified = apply();
      if (simplified) {
	Element *result = simplified->compare(_ele, unknwons);
	simplified->remove();
	return result;
      }
      else {
	Element *simplified2 = e->apply();
	if (simplified2) {
	  Element *result = compare(simplified2, unknwons);
	  _ele->remove();
	  _ele = simplified2;	  
	  return result;
	}
	else
	  return false;
      }
    }
  }
  else {
    Element* simplified = apply();
    if (simplified) {
      Element *result = simplified->compare(_ele, unknwons);
      simplified->remove();
      return result;
    }
  }
}


bool Application::equals_ex(Element* ele2)
{
  Element *simplified = apply();
  bool result;
  if (simplified) {
    result = simplified->equals(ele2);
    simplified->remove();    
  }
  else
    result = false;
  return result;
}



Element* Application::type()
{
  Element *t = f->type();
  ForAll* ft = t->cast< ForAll > (); 
  t->remove();
  Element* result = ft->getTerm();
  Element* result2 = result->replace(var, 0);
  ft->remove();
  result->remove();
  return result2;
}
