#include "variable.h"
#include "abstraction.h"
#include "application.h"
#include <sstream>

bool Variable::isUsed(int index)
{
  return this->index == index;
}

void Variable::toString(std::ostream &stream, std::vector< std::string >& stringMapping, bool klammern)
{
  if (stringMapping.size() <= index)
    stream << "[[" << index << "]]";
  else
    stream << stringMapping.at(stringMapping.size() - index - 1);
}

bool Abstraction::isUsed(int index)
{
  return var->isUsed(index) || term->isUsed(index+1);
}

void Abstraction::toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern)
{
  if (klammern) stream << "(";
  static int nr = 0;
  ++nr;
  bool usage = term->isUsed(0);
  std::stringstream name;
  if (usage)
    name << "N" << nr;
  else
    name << "_BUG_";
  if (usage) {
    stream << name.str() << ": ";
  }
  var->toString (stream, stringMapping, true);
  stream << " " << stringType() << " ";
  stringMapping.push_back(name.str());
  term->toString(stream, stringMapping, false);
  stringMapping.pop_back();
  if (klammern) stream << ")";
}

bool Application::isUsed(int index)
{
  return f->isUsed(index) || var->isUsed(index);
}

void Application::toString(std::ostream& stream, std::vector< std::string >& stringMapping, bool klammern)
{
  if (klammern) stream << "(";
  
  f->toString(stream, stringMapping, true);
  stream << " ";
  var->toString(stream, stringMapping, true);
  if (klammern) stream << ")";
}


