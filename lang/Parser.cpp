#include "Parser.h"
#include "kern/element_ptr.h"
#include <kern/ext/named_var.h>
#include "kern/element_ptr.ext.h"


int Parser::lex()
{
  while (true) {
    if (!lex_front)
      lex_front = stream.get();
    if (stream.eof())
      return 0;
    
    int l1 = lex_front;
    lex_front = 0;
    
    int l2 = 0;
    
    switch (l1) {
      case ' ':
      case '\t':
      case '\n':
      case '\r':
	continue;
      case '/':
	l2 = stream.get();
	if (l2 == '/') {
	  //Kommentar
	  while (stream.get() != '\n');
	  continue;
	}
	else {
	  lex_front = l2;
	  return '/';
	}
      case ':':
	l2 = stream.get();
	if (l2 == '=')
	  return ZUWEISUNG;
	lex_front = l2;
      case '(':
      case ')':
      case ';':
	return l1;
      case '-':
      case '=':
	l2 = stream.get();
	if (l2 == '>')
	  return l1 == '-' ? ARROW : DOUBLE_ARROW;
      default:
	std::string literal;
	literal.push_back(l1);
	if (!l2) l2 = stream.get();
	while (!stream.eof() && l2 != ' ' && l2 != '\t' && l2 != ':' && l2 != ' ' && l2 != '\n' && l2 != '\t' && l2 != ';' && l2 != '(' && l2 != ')') {
	  literal.push_back(l2);
	  l2 = stream.get();
	}
	lex_front = l2;
	if (literal == "print") return PRINT;
	if (literal == "Set") 
	  d_val__ = Creater::set();
	else {
	  std::map< std::string, ElementPtr >::iterator it = eles.find(literal);
	  if (it == eles.end())
	    d_val__ = Creater::namedVariable (literal);
	  else
	    d_val__ = it->second;
	}
	return LITERAL;
    }
  }
}

void Parser::setLiteral(std::string arg1)
{
  d_val__ = (new NamedVar< std::string > (arg1))->copy();
}


std::string Parser::literal(ElementPtr ptr)
{
  NamedVar< std::string > &a = dynamic_cast< NamedVar< std::string > & > (*ptr);
  return a.name();
}
