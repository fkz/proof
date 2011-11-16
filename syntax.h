#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <typeinfo>

/**
 * @class Element
 * Represents a syntactic element in the tree
 * Should not be altered, unless locked
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
    if (!--refCount)
      delete this;
  }
  
protected:
  Element() : refCount(0) { }
  
    /// Should remove all elements in connection to this
  virtual ~Element() { }
public:
  bool check() {
    std::vector< Element * > e;
    return check(e);
  }
  virtual bool check(std::vector< Element * > &vars) = 0;  
  virtual Element *step (int from) = 0;
  virtual Element *replace (Element *with, int varId) = 0;
  /// Should return the same element (of cause @ref refCount is different)
  virtual Element *clone() = 0;  
  virtual Element *type() = 0;
  virtual bool equals (Element *ele2) = 0;
  virtual bool equals_ex(Element* ele2) { return false;}
  
  
  ///Functions to do pretty printing
  virtual bool isUsed(int index) = 0;
  virtual void toString (std::ostream &stream, std::vector< std::string > &stringMapping, bool klammern) = 0;
  
  template< class T >
  T *cast ();
  
};

class Variable: public Element {
private:
  unsigned int index;
  Element *_type;
protected:
  virtual Element* type();
  virtual Element* replace(Element* with, int varId);
  virtual Element* step(int from);
  virtual Element* clone();
  virtual bool equals(Element* ele2);
public:
  Variable (int index, Element *type) : index(index), _type(type) { }
  
  virtual bool isUsed(int index);
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern);
  virtual bool check(std::vector< Element* >& vars);
};

class Application: public Element {
private:
  Element *f;
  Element *var;
protected:
  virtual Element* type();
  virtual Element* replace(Element* with, int varId);
  virtual Element* step(int from);
  virtual ~Application();
  virtual Element* clone();
  virtual bool equals(Element* ele2);
  virtual bool equals_ex(Element* ele2);
  virtual bool check(std::vector< Element* >& vars);
public:
  Application (Element *f, Element *var): f (f), var (var) { }
  
  virtual bool isUsed(int index);
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern);
  Element *apply();
};

class Abstraction: public Element {
private:
  Element *var;
  Element *term;
protected:
  virtual Element* replace(Element* with, int varId);
  virtual Element* step(int from);
  virtual ~Abstraction();
  virtual Element* clone();
  virtual  Abstraction *_clone(Element* newTerm, Element* newVar) = 0;
  virtual bool equals(Element* ele2);
  Abstraction (Element *var, Element *term) : var(var), term(term) { }
  virtual bool check(std::vector< Element* >& vars);
public:
  Element *getVar() {
    return var->copy();
  }
  
  Element *getTerm() {
    return term->copy();    
  }
  
  virtual bool isUsed(int index);
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern);
  virtual std::string stringType() = 0;
};

class Function: public Abstraction {
public:
  virtual Abstraction* _clone(Element* newTerm, Element* newVar);
  virtual Element* type();
  
  virtual std::string stringType() { return "=>"; }
  Function (Element *var, Element *term) : Abstraction(var, term) {}
};

class ForAll: public Abstraction {
public:
  virtual Abstraction* _clone(Element* newTerm, Element* newVar);
  virtual Element* type();
  virtual std::string stringType() { return "->"; }
  
  ForAll (Element *var, Element *term) : Abstraction(var, term) {}
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
    virtual bool equals(Element* ele2) { return typeid(*this)==typeid(*ele2) || equals_ex(ele2); }
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
    virtual bool equals(Element* ele2) { return typeid(*ele2) == typeid(Prop) || equals_ex(ele2); }
    Prop() {}  
};

template< class T >
T* Element::cast()
{
  T *result = dynamic_cast< T * > (this);
  if (!result) {
    Application *appl = dynamic_cast< Application * > (this);
    if (!appl)
      return 0;
    Element* ele = appl->apply();
    result = ele->cast< T > ();
    ele->remove ();
  }
  return static_cast< T * > (result->copy());
}


namespace Creater{
  
Element *variable (int index, Element *type);
Element *function (Element *var, Element *f);
Element *forAll (Element *var, Element *f);
Element *application (Element *f, Element *var);
Element *set ();
Element *prop ();

void print (Element *e);

};