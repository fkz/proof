#include "Parser.h"
#include "kern/element_ptr.h"
#include <kern/ext/named_var.h>
#include "kern/element_ptr.ext.h"
#include "additional_elements.h"
#include <kern/constant.h>


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
	  while (stream.get() != '\n') {
	    if (stream.eof())
	      break;
	  }
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
      case '?':
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
	d_val__ = (new Literal (literal))->copy();
	return LITERAL;
    }
  }
}

ElementPtr Parser::evaluate(const std::string& string)
{
  if (string == "Set")
    return Creater::set();
  
  std::map< std::string, ElementPtr >::iterator it = eles.find(string);
  if (it == eles.end())
    return Creater::namedVariable(string);
  else
    return it->second;
}


void Parser::setLiteral(std::string arg1)
{
  d_val__ = Creater::namedVariable (arg1);
}


std::string Parser::literal(ElementPtr ptr)
{
  NamedVar< std::string > &a = dynamic_cast< NamedVar< std::string > & > (*ptr);
  return a.name();
}

void Parser::set(const std::string& name, ElementPtr ele, ElementPtr type, bool unfoldable)
{
  int unique = names.size();
  names.push_back(name);
  Element *mtype;
  if (!&*type)
    mtype = ele->type();
  else
    mtype = type->copy();

  eles[ name ] = (new Constant (unique, mtype, name, unfoldable ? 0 : ele->copy()))->copy();
}



ElementPtr Parser::buildForAlls(ElementPtr& vars, ElementPtr& aussage)
{
  std::list< std::pair< std::string, ElementPtr > > varList = ArgumentList::getList(vars);
  ElementPtr fun = aussage;
  for (std::list< std::pair< std::string, ElementPtr > >::reverse_iterator it = varList.rbegin(); it != varList.rend(); ++it) {
    fun = Creater::namedForAll (it->first, it->second, fun);
  }
  return fun;
}

ElementPtr Parser::buildFunctions(ElementPtr& vars, ElementPtr& aussage)
{
  std::list< std::pair< std::string, ElementPtr > > varList = ArgumentList::getList(vars);
  ElementPtr fun = aussage;
  for (std::list< std::pair< std::string, ElementPtr > >::reverse_iterator it = varList.rbegin(); it != varList.rend(); ++it) {
    fun = Creater::namedFunction (it->first, it->second, fun);
  }
  return fun;
}
