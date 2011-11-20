#include "unknown.h"
#include "../element_cast.h"

int Unknown::count = 0;

Element* Unknown::replaceNamed(Element* with, int T1, void* T2)
{
  if (T1 == REPLACE_ELEMENT) {
    Element *ele = static_cast< Element * >(T2);
    Unknown *unk = ele->cast< Unknown >();
    if (unk) {
      Element *result = unk->i == i ? with->copy() : copy();
      unk->remove();
      return result;
    }
    else
      return copy();
  }
  else
    return copy();
}