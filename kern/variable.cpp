#include "variable.h"

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

Element* Variable::replaceNamed(Element* with, int T1, void* T2)
{
  return copy();
}


Element* Variable::step(int from)
{
  if (index >= from) {
    return (new Variable (index + 1, _type->copy()))->copy();
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

Variable::~Variable()
{
  _type->remove();
}
