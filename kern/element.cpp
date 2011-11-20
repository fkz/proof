#include "element.h"
#include "ext/unknown.h"

Element* Element::compareType(Element* _type) {
  if (Unknown::isUnknown(_type))
    return copy();
  else {
    Element* myt = type();
    Element *result;
    if (myt->equals (_type))
      result = copy();
    else
      result = 0;
    myt->remove();
    return result;
  }
}
