#include "variable.h"
#include "element_cast.h"

Element* Variable::type()
{
  return _type->copy();
}

Element* Variable::replace(Element* with, int varId)
{
  // type
  Element* type2 = _type->replace (with, varId);
  // ...
  if (varId <= index) {
    if (varId == index) {
      type2->remove();
      return with->copy();
    }
    else
      return (new Variable (index-1, type2))->copy();
  }
  else {
    if (type2 == _type) {
      type2->remove();
      return copy();
    }
    else {
      return (new Variable (index, type2))->copy();
    }
  }
}

Element* Variable::replaceNamed(Element* with, int T1, void* T2)
{
  Element* t2 = _type->replaceNamed(with, T1, T2);
  if (t2 == _type) {
    t2->remove();
    return copy();
  }
  else {
    return (new Variable (index, t2))->copy();
  }
}


Element* Variable::step(int from)
{
  if (index >= from) {
    return (new Variable (index + 1, _type->step(from)))->copy();
  }
  return copy();
}

Element* Variable::applyRecursive()
{
  return copy();
}


Element* Variable::clone()
{
  return new Variable(index, _type->copy());
}

bool Variable::equals(Element* ele2)
{
  Variable* e = ele2->cast< Variable > ();
  if (!e) return false;
  bool result = index == e->index;
  e->remove();
  return result;
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
