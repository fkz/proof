#include "element.h"
#include "ext/unknown.h"
#include <vector>
#include "element_cast.h"
#include "ext/dummy.h"

Element *Element::compare(Element *&_ele, std::vector< std::pair< Unknown*, Element* > >& unknwons)
{
  Unknown *unkT;
  bool not_finnished = true;
  while (not_finnished) {
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
  else {
    Dummy *dummy = _ele->cast< Dummy > ();
    if (dummy) {
      Element *thisC = copy();
      _ele->remove();
      _ele = dummy->_compare(thisC, unknwons);
      if (_ele)
	return thisC;
      else {
	//TODO: this is probably quite bad; but in case of false return nothing is defined
	_ele = thisC;
	return 0;
      }
    }
  }

  return _compare(_ele, unknwons);  
}