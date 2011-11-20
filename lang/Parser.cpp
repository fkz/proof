#include "Parser.h"
#include "kern/element_ptr.h"
#include <kern/ext/named_var.h>
#include "kern/element_ptr.ext.h"
#include "additional_elements.h"
#include <kern/constant.h>
#include "lex.h"

int Parser::lex()
{
  lexer.lex(d_val__); 
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
