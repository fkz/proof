#include "Parser.h"
#include "kern/element_ptr.h"
#include <kern/ext/named_var.h>
#include "kern/element_ptr.ext.h"


int Parser::lex()
{
  d_val__ = Creater::set();
  static int i = 0;
  switch (++i) {
    case 1: return PRINT;
    case 2: 
      setLiteral("a");
      return LITERAL;
    case 3: return ':';
    case 4: 
      d_val__ = Creater::set();
      return LITERAL;
    case 5: return ARROW;
    case 6: 
      setLiteral("a");
      return LITERAL;
    case 7:
      return ARROW;
    case 8:
      setLiteral("a");
      return LITERAL;
    case 9:
      return ';';
    case 10:
      return 0;
  }
}

void Parser::setLiteral(std::string arg1)
{
  d_val__ = (new NamedVar< std::string > (arg1))->copy();
}


Parser::Parser(std::istream& str)
{

}

std::string Parser::literal(ElementPtr ptr)
{
  NamedVar< std::string > &a = dynamic_cast< NamedVar< std::string > & > (*ptr);
  return a.name();
}
