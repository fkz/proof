#pragma once
#include "../simple_element.h"

class Unknown: public SimpleElement {
public:
  virtual Element* clone() {
    return new Unknown();
  }
  virtual void toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern) {
    stream << "Unknown";
  }
  virtual Element* type() {
    return copy();
  }
};