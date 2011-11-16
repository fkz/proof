#pragma once
#include "element.h"

class Abstraction: public Element {
private:
  Element *var;
  Element *term;
protected:
  virtual Element* replace(Element* with, int varId);
  virtual Element* replaceNamed(Element* with, int T1, void* T2);
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