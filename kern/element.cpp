#include "element.h"
#include "ext/unknown.h"
#include <vector>
#include "element_cast.h"

Element *Element::compare(Element *&_ele, std::vector< std::pair< Unknown*, Element* > >& unknwons)
{
  Unknown *unkT;
  bool not_finnished = true;
  while (not_finnished) {
    R1:
    unkT = _ele->cast< Unknown >();
    if (!unkT)
      break;
    not_finnished = false;
    for (std::vector< std::pair< Unknown*, Element* > >::iterator it = unknwons.begin(); it != unknwons.end(); ++it) {
      if (it->first->equals(unkT)) {
	unkT->remove();
	_ele = it->second;
	not_finnished = true;
	break;
      }
    }
  }
  if (unkT) {
    unknwons.push_back(std::make_pair (unkT, copy()));
    _ele->remove();
    _ele = copy();
    return copy();
  }

  _compare(_ele, unknwons);  
}