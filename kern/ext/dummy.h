#pragma once
#include "../element.h"

class Dummy: public Element {
private:
  Element *_type;
  
public:
  virtual Element* applyRecursive() {
    return copy();
  }
  virtual bool check(std::vector< Element* >& vars) {
    return false;
  }
  virtual bool equals(Element* ele2) {
    Dummy* ele2_ = ele2->cast< Dummy >();
    if (ele2_) {
	bool result = _type->equals(ele2_->_type);
	ele2_->remove();
	return result;
    }
    else
      return false;
  }
  virtual bool isUsed(int index) {
    return _type->isUsed(index);
  }
  virtual Element* replace(Element* with, int varId) {
    Element* _type_ = _type->replace(with, varId);
    if (_type_ != _type) {
      return (new Dummy(_type_))->copy();
    }
    else {
      _type_->remove();
      return copy();
    }
  }
  virtual Element* replaceNamed(Element* with, int T1, void* T2) {
    Element* _type_ = _type->replaceNamed(with, T1, T2);
    if (_type_ != _type) {
      return (new Dummy(_type_))->copy();
    }
    else {
      _type_->remove();
      return copy();
    }
  }
  virtual Element* step(int from) {
    Element* _type_ = _type->step(from);
    if (_type_ != _type) {
      return (new Dummy(_type_))->copy();
    }
    else {
      _type_->remove();
      return copy();
    }
  }
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) {
    stream << "Dummy[T:";
    _type->toString(stream, stringMapping, false);
    stream << "]";
  }
  virtual Element* type() {
    return _type->copy();
  }
  virtual Element* _compare(Element*& _ele, std::vector< std::pair< Unknown*, Element* > >& unknwons);
  
public:
  Dummy (Element *_type) : _type(_type)  { }
};