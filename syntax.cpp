#include "syntax.h"
#include <vector>
#include <string>
#include <sstream>
#include <typeinfo>
#include <iostream>

Element* Variable::type()
{
  return _type->copy();
}

Element* Variable::replace(Element* with, int varId)
{
  if (varId <= index) {
    if (varId == index)
      return with->copy();
    else
      return (new Variable (index-1, _type->copy()))->copy();
  }
  else
    return copy();
}

Element* Variable::step(int from)
{
  if (index >= from) {
    return new Variable (index + 1, _type->copy());
  }
  return copy();
}

Element* Variable::clone()
{
  return new Variable(index, _type->copy());
}

bool Variable::equals(Element* ele2)
{
  Variable* e = dynamic_cast< Variable * > (ele2);
  if (!e) return ele2->equals_ex (this);
  return index == e->index;
}

bool Variable::check(std::vector< Element* >& vars)
{
  if (vars.size() <= index)
    return false;
  if (!_type->check(vars))
    return false;
  return _type->equals(vars[vars.size()-index-1]);
}




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
    return new Application(newF, newVar);
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
  ForAll* all = f->type()->cast< ForAll >();
  bool result = all->getVar()->type()->equals(var->type());
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

Abstraction* ForAll::_clone(Element* newTerm, Element* newVar)
{
  return new ForAll(newTerm->copy(), newVar->copy());
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
  return new Function(newTerm->clone(), newVar->clone());
}

Element* Function::type()
{
  Element* t = getTerm();
  ForAll *result = new ForAll (getVar(), t->type());
  t->remove();
  return result->copy();
}


// pretty printing

bool Variable::isUsed(int index)
{
  return this->index == index;
}

void Variable::toString(std::ostream &stream, std::vector< std::string >& stringMapping, bool klammern)
{
  if (stringMapping.size() <= index)
    stream << "[[" << index << "]]";
  else
    stream << stringMapping.at(stringMapping.size() - index - 1);
}

bool Abstraction::isUsed(int index)
{
  return var->isUsed(index) || term->isUsed(index+1);
}

void Abstraction::toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern)
{
  if (klammern) stream << "(";
  static int nr = 0;
  ++nr;
  bool usage = term->isUsed(0);
  std::stringstream name;
  if (usage)
    name << "N" << nr;
  else
    name << "_BUG_";
  if (usage) {
    stream << name.str() << ": ";
  }
  var->toString (stream, stringMapping, true);
  stream << " " << stringType() << " ";
  stringMapping.push_back(name.str());
  term->toString(stream, stringMapping, false);
  stringMapping.pop_back();
  if (klammern) stream << ")";
}

bool Application::isUsed(int index)
{
  return f->isUsed(index) || var->isUsed(index);
}

void Application::toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern)
{
  if (klammern) stream << "(";
  
  f->toString(stream, stringMapping, true);
  stream << " ";
  var->toString(stream, stringMapping, true);
  if (klammern) stream << ")";
}

namespace Creater{
  
Element *variable (int index, Element *type) {
  return (new Variable (index, type->copy()))->copy();
}

Element *function (Element *var, Element *f) {
  return (new Function (var->copy(), f->copy()))->copy();
}

Element *forAll (Element *var, Element *f) {
  return (new ForAll (var->copy(), f->copy()))->copy();
}

Element *application (Element *f, Element *var) {
  return (new Application (f->copy(), var->copy()))->copy();
}

Element* set()
{
  return (new TheTypeOfAllTypesTM())->copy();
}

Element* prop()
{
  return (new Prop())->copy();
}


void print (Element *ele) {
  if (!ele->check ()) {
    std::cout << "NOT VALID(!)" << std::endl;
  }
  else
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
  
};






