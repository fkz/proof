#include "dummy.h"
#include "../abstraction.h"
#include "../application.h"
#include "named_var.h"

struct Error {
public:
  bool operator == (const Error &err2) { return false; }  
};

std::ostream &operator << (std::ostream &stream, const Error &);



Element* Dummy::_compare(Element*& _ele, std::vector< std::pair< Unknown*, Element* > >& unknwons)
{
  Function* appl = _ele->cast< Function > ();
  if (appl) {
    ForAll* forAll = _type->cast< ForAll > ();
    if (forAll) {
      Element *applVar = appl->getVar();
      Element *allVar = forAll->getVar();
      Element *allVar2 = allVar->compare(applVar, unknwons);
      allVar->remove();
      
      for (std::vector< std::pair< Unknown*, Element* > >::iterator it = unknwons.begin(); it != unknwons.end(); ++it) {
	Element* t = it->second->step(0);
	it->second->remove();
	it->second = t;
      }
      Element *applTerm = appl->getTerm();
      Element *allTerm = forAll->getTerm();
      Element *allTermDummy = new Dummy (allTerm);
      allTermDummy->copy();
      Element *allTermDummy2 = allTermDummy->compare(applTerm, unknwons);
      allTermDummy->remove();
      Element *error = new NamedVar<Error>(Error());
      error->copy();
      for (std::vector< std::pair< Unknown*, Element* > >::iterator it = unknwons.begin(); it != unknwons.end(); ++it) {
	Element* t = it->second->replace(error, 0);
	it->second->remove();;
	it->second = t;
      }
      
      error->remove();
      if (!(allTermDummy2 && allVar2))
	return 0;

      Element *allTermDummy2Type = allTermDummy2->cast<Dummy>()->type();

      Element* result = (new Dummy ((new ForAll (allVar2, allTermDummy2Type))->copy()))->copy();
      _ele = new Function (applVar, applTerm);
      _ele = _ele->copy();
      
      return result->copy();
    }
  }
  else {
    Element* _ele_type = _ele->type();
    Element* _type2 = _type->compare(_ele_type, unknwons);
    if (!_type2)
      return 0;
    for (std::vector< std::pair< Unknown*, Element* > >::iterator it = unknwons.begin(); it != unknwons.end(); ++it) {
      _ele = _ele->replaceNamed(it->second, Unknown::REPLACE_ELEMENT, it->first);
    }
    Dummy* result = new Dummy(_type2);
    return result->copy();
  }
}
