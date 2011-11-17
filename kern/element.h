#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <cassert>

/**
 * @class Element
 * Represents a syntactic element in the tree
 * Should not be altered (normally)
 */
class Element {
private:
  int refCount;
public:
  Element *copy () {
    ++refCount;
    return this;
  }
  
  void remove() {
    //if (refCount <= 0)
    //  assert(0);
    if (!--refCount)
      delete this;
  }
  
protected:
  Element() : refCount(0) { }
  
    /// Should remove all elements in connection to this
  virtual ~Element() { }
public:
  virtual bool equals_really(Element *ele2) {
    Element* t = type();
    Element* t2 = ele2->type();
    bool result = t->equals_really(t2);
    t->remove();
    t2->remove();
    return result && equals(ele2);
  }
  
  bool check() {
    std::vector< Element * > e;
    return check(e);
  }
  
  virtual Element *apply() {
    return 0;
  }
  virtual Element *applyRecursive () = 0;
  virtual bool check(std::vector< Element * > &vars) = 0;  
  virtual Element *step (int from) = 0;
  virtual Element *replace (Element *with, int varId) = 0;
  /// Should return the same element (of cause @ref refCount is different)
  virtual Element *clone() = 0;  
  virtual Element *type() = 0;
  virtual bool equals (Element *ele2) = 0;
  
  ///For the conversion phase, this replacements can be used (not used in any checking)
  virtual Element *replaceNamed (Element *with, int T1, void *T2) = 0;
  
  
  ///Functions to do pretty printing
  virtual bool isUsed(int index) = 0;
  virtual void toString (std::ostream &stream, std::vector< std::string > &stringMapping, bool klammern) = 0;
  
  template< class T >
  T *cast ();
  
};




class TheTypeOfAllTypesTM: public Element {
public:
    virtual bool check(std::vector< Element* >& vars) { return true; }
    virtual Element* clone() { return new TheTypeOfAllTypesTM(); }
    virtual bool isUsed(int index) { return false; }
    virtual Element* replace(Element* with, int varId) { return copy(); }
    virtual Element* step(int from) { return copy(); }
    virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) { stream << "SET"; }
    virtual Element* type() { return copy(); } //!!!!! THIS IS PROBABLY INCONSISTENT 
    virtual bool equals(Element* ele2) { 
        TheTypeOfAllTypesTM* cc = ele2->cast< TheTypeOfAllTypesTM >();
	if (cc) cc->remove();
	return cc;
    }
    virtual Element* replaceNamed(Element* with, int T1, void* T2) { return copy(); }
    virtual bool equals_really(Element* ele2) { return equals(ele2); } 
    virtual Element* applyRecursive() { return copy(); }
    TheTypeOfAllTypesTM() {}
};

class Prop: public Element {
public:
    virtual bool check(std::vector< Element* >& vars) { return true; }
    virtual Element* clone() { return new TheTypeOfAllTypesTM(); }
    virtual bool isUsed(int index) { return false; }
    virtual Element* replace(Element* with, int varId) { return copy(); }
    virtual Element* step(int from) { return copy(); }
    virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) { stream << "PROP"; }
    virtual Element* type() { return (new TheTypeOfAllTypesTM())->copy(); } 
    virtual bool equals(Element* ele2) { 
        Prop* result = ele2->cast< Prop >();
	if (result) result->remove();
	return result;
    }
    virtual Element* replaceNamed(Element* with, int T1, void* T2) { return copy(); }
    virtual bool equals_really(Element* ele2) { return equals (ele2); }
    virtual Element* applyRecursive() { return copy(); }
    Prop() {}  
};
