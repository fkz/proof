#include "kern/simple_element.h"
#include "kern/element_ptr.h"
#include <list>

class Additional: public SimpleElement {
protected:
  virtual Element* clone() { return 0; }
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) { stream << "__BUG__ADDITIONAL"; }
  virtual Element* type() { return 0; }
  virtual bool equals(Element* ele2) { return false; }
};

class LiteralList: public Additional {
private:
  std::list< std::string > list;
public:
  static LiteralList *create (std::string name) {
    LiteralList* result = new LiteralList ();
    result->copy();
    result->list.push_back(name);
    return result;
  }
  static void push (ElementPtr ptr, std::string n) {
    LiteralList &ll = *dynamic_cast< LiteralList * > (&*ptr);
    ll.list.push_back(n);
  }
  friend class ArgumentList;
};

class ArgumentList: public Additional {
public:
  std::list< std::pair< std::string, ElementPtr > > list;
  
public:
  static ArgumentList *create (std::string name, ElementPtr ele) {
    ArgumentList* result = new ArgumentList();
    result->copy();
    result->list.push_back(std::make_pair (name, ele));
    return result;
  }
  
  static ArgumentList *createMany (ElementPtr litList, ElementPtr ele) {
    ArgumentList *result = new ArgumentList();
    result->copy();
    LiteralList *ll = dynamic_cast< LiteralList * > (&*litList);
    for (std::list< std::string >::const_iterator it = ll->list.begin(); it != ll->list.end(); ++it)
      result->list.push_back(std::make_pair (*it, ele));
    return result;
  }
  static ArgumentList *createManyUnknowns (ElementPtr litList) {
    ArgumentList *result = new ArgumentList();
    result->copy();
    LiteralList *ll = dynamic_cast< LiteralList * > (&*litList);
    for (std::list< std::string >::const_iterator it = ll->list.begin(); it != ll->list.end(); ++it)
      result->list.push_back(std::make_pair (*it, Creater::unknown()));
    return result;
  }
  
  
  static ElementPtr push (ElementPtr pt1, ElementPtr pt2) {
    ArgumentList* e1 = dynamic_cast< ArgumentList * > (&*pt1);
    ArgumentList* e2 = dynamic_cast< ArgumentList * > (&*pt2);
    e1->list.insert(e1->list.end(), e2->list.begin(), e2->list.end());
    ElementPtr result = e1->copy();
    return result;
  }
  static   std::list< std::pair< std::string, ElementPtr > > getList (ElementPtr ptr) {
    return dynamic_cast< ArgumentList * > (&*ptr)->list;
  }
};

class Literal: public Additional {
private:
  std::string lit;
public:
  Literal (const std::string &l) : lit(l) { }
  static std::string from(ElementPtr &ele) {
    return dynamic_cast< Literal * > (&*ele)->lit;
  }
};