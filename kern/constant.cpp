#include "constant.h"

bool Constant::check(std::vector< Element* >& vars)
{
  // remark, that outstanding variables are NOT allowed in a constant, so don't use vars
  std::vector< Element * > vars2;
  
  if (!_type->check(vars2)) return false;
  if (unfold) {
    if (!unfold->check(vars2))
      return false;
    if (!_type->equals(unfold->type()))
      return false;
  }
  return true;
}

Element* Constant::clone() {
  return new Constant (unique_id, _type->copy(), unfold ? unfold->copy() : 0);
}

bool Constant::equals(Element* ele2) {
  Constant *ele = ele2->cast<Constant> ();
  if (!ele) {
    if (unfold)
      return unfold->equals(ele2);
    else
      return false;
  }
  else {
    if (ele->unique_id == unique_id)
      return true;
    if (unfold)
      return unfold->equals(ele);
    else if (ele->unfold)
      return ele->unfold->equals(this);
    else
      return false;
  }
}

Element* Constant::applyRecursive()
{
  if (unfold)
    return unfold->copy();
  else
    return copy();
}

bool Constant::isUsed(int index)
{
  return false;
}

Element* Constant::replace(Element* with, int varId)
{
  return copy();
}

Element* Constant::replaceNamed(Element* with, int T1, void* T2)
{
  return copy();
}

Element* Constant::step(int from)
{
  return copy();
}

void Constant::toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern)
{
  stream << "CONST[" << unique_id << "]{";
  if (unfold)
    unfold->toString(stream, stringMapping, false);
  stream << "}";
}

Element* Constant::type()
{
  return _type->copy();
}

Element* Constant::apply()
{
  return unfold->copy();
}








