#pragma once
#include "kern/element.h"
#include <map>
#include <list>

/*
class NamedVar: public Element {
private:
  std::string name;
public:
  NamedVar (const std::string &s) : name(s) { }
  
  virtual bool check(std::vector< Element* >& vars) { return false; }
  virtual Element* clone() { return new NamedVar (name); }
  virtual bool equals(Element* ele2) { return false; }
  virtual bool isUsed(int index) { return false; }
  virtual Element* replace(Element* with, int varId) { 
    return copy();
  }
  virtual Element* step(int from) { return copy(); }
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) { stream << "NAMED<" << name << ">"; }
  virtual Element* type() { return Creater::unknown()->copy(); }
};

/*
class Semantic {
private:
  class SemanticClass {
  private:
    int refCount;
    ElementPtr ele;
    std::string literal;
    
    enum Mode {
      Ele,
      Lit
    } mode;
    
        
  public:
    SemanticClass () : refCount(1), ele(0), mode(Lit) { }
    
    SemanticClass *unlock() {
      if (refCount == 1)
	return this;
      else {
	SemanticClass *result = clone();
	deref();
	return result;
      }
    }
    void deref() {
      if (--refCount == 0)
	delete this;
    }
    SemanticClass *copy() {
      ++refCount;
      return this;
    }
    
    SemanticClass *clone() const {
      SemanticClass* result = new SemanticClass();
      result->ele = ele;
      result->literal = literal;
      return result;
    }
    
    void normalize() {
      if (mode == Lit) {
	ele = (new NamedVar (literal))->copy();
	mode = Ele;
	literal.clear();
      }
    }
    
  };
  SemanticClass *semantic;
public:
  void unlock() {
    semantic = semantic->unlock();
  }
  
  Semantic() : semantic (new SemanticClass()) { }
  
  Semantic(const Semantic &s) : semantic (s.semantic->copy()) {
  }
  
  ~Semantic() {
    semantic->deref();
  } 
};*/