#pragma once
#include <iosfwd>
#include "kern/element_ptr.h"
#include <list>

class Lexer {
public:
  Lexer (std::istream &stream);
  int lex(ElementPtr &ptr);
private:
  int nextToken (ElementPtr& d_val__, bool simple = false);
  std::list< std::pair< int, ElementPtr > > nextTokens;
  std::istream &stream;
  int lex_front;
};