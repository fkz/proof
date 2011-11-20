#include "constant.h"
#include "element_cast.h"

bool Constant::check(std::vector< Element* >& vars)
{
  // remark, that outstanding variables are NOT allowed in a constant, so don't use vars
  std::vector< Element * > vars2;
  
  if (!_type->check(vars2)) return false;
  if (unfold) {
    if (!unfold->check(vars2))
      return false;
    Element* unfold_type = unfold->type();
    if (!_type->equals(unfold_type)) {
      unfold_type->remove();
      return false;
    }
    unfold_type->remove();
  }
  return true;
}


Element* Constant::clone() {
  return new Constant (unique_id, _type->copy(), str, unfold ? unfold->copy() : 0);
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

bool Constant::_compare(Element* _ele, std::vector< std::pair< Unknown*, Element* > >& unknwons)
{
  Constant *ele = _ele->cast<Constant> ();
  if (!ele) {
    if (unfold)
      return unfold->compare(_ele, unknwons);
    else
      return false;
  }
  else {
    if (ele->unique_id == unique_id)
      return true;
    if (unfold)
      return unfold->compare(_ele, unknwons);
    else if (ele->unfold)
      return ele->unfold->compare(this, unknwons);
    else
      return false;
  }
}


Element* Constant::applyRecursive()
{
  if (unfold)
    return unfold->applyRecursive();
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
  stream << str;
  /*stream << "CONST[" << unique_id << "]{";
  if (unfold)
    unfold->toString(stream, stringMapping, false);
  stream << "}";*/
}

Element* Constant::type()
{
  return _type->copy();
}

Element* Constant::apply()
{
  if (unfold)
    return unfold->copy();
  else
    return 0;
}








