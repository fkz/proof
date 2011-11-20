#include "Parser.h"
#include "kern/element_ptr.h"
#include <kern/ext/named_var.h>
#include "kern/element_ptr.ext.h"
#include "additional_elements.h"
#include <kern/constant.h>
#include "lex.h"
#include <kern/ext/dummy.h>

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


void Parser::doChecking(ElementPtr& type, ElementPtr& term, const std::string &str)
{
  std::vector< std::pair< Unknown *, Element * > > unknowns;
  Element *type_ = (new Dummy(type->copy()))->copy();
  term = term->compare(type_, unknowns);
  type = type_->type();
  type_->remove();
  if (!&*term) {
    std::cerr << "Comparing fails (defining " << str << ")" << std::endl;
  }

  for (std::vector< std::pair< Unknown*, Element* > >::iterator it = unknowns.begin(); it != unknowns.end(); ++it) {
    (ElementPtr)it->first;
    (ElementPtr)it->second;
  }
  
  bool r = &*term;
  if (r) {
    if (!term->check()) { r = false; std::cerr << "error defining " << str << std::endl; }
    if (!type->check()) { r = false;std::cerr << "error defining " << str << "(wrong type)" << std::endl; }
    ElementPtr term_type = term->type();
    term_type = term->type();
    if (r && !term_type->equals_really(&*type)) { 
      r = false;
      std::cerr << "error defining " << str << "(types differ)" << std::endl; 
      Creater::print(term_type);
      Creater::print(type);
    }
  }
  
  if (r)  set (str, term, type);  
}
