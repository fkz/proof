#include "lex.h"
#include "Parserbase.h"
#include "additional_elements.h"

Lexer::Lexer(std::istream& stream)
: stream(stream), lex_front(0), line(0)
{
  
}


int Lexer::lex(ElementPtr& ptr)
{
  if (nextTokens.empty()) {
    return nextToken(ptr);
  }
  else {
    std::pair< int, ElementPtr > result = nextTokens.front();
    nextTokens.pop_front();
    ptr = result.second;
    return result.first;
  }
}



int Lexer::nextToken(ElementPtr& d_val__, bool simple)
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
      case '\n':
	++line;
      case ' ':
      case '\t':
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
	  return Parser::ZUWEISUNG;
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
	  return l1 == '-' ? Parser::ARROW : Parser::DOUBLE_ARROW;
      default:
	std::string literal;
	literal.push_back(l1);
	if (!l2) l2 = stream.get();
	while (!stream.eof() && l2 != ' ' && l2 != '\t' && l2 != ':' && l2 != ' ' && l2 != '\n' && l2 != '\t' && l2 != ';' && l2 != '(' && l2 != ')') {
	  literal.push_back(l2);
	  l2 = stream.get();
	}
	lex_front = l2;
	if (literal == "print") return Parser::PRINT;
	d_val__ = (new Literal (literal))->copy();
	if (simple)
	  return Parser::LITERAL;
	else {
	  int finnish = 0;
	  const int FINNISH1 = 1;
	  const int FINNISH2 = 2;
	  int nested = 0;
	  while (finnish == 0) {
	    ElementPtr nextPtr;
	    int next = nextToken(nextPtr, true);
	    nextTokens.push_back(std::make_pair(next, nextPtr));
	    switch (next) {
	      case ':':
	      case Parser::DOUBLE_ARROW:
		finnish = FINNISH2;
		break;
	      case '(':
		if (++nested >= 2)
		  finnish = FINNISH1;		
		break;
	      case ')':
		if (nested-- == 0)
		  finnish = FINNISH1;
		break;
	      case Parser::LITERAL:
		continue;
	      default:
		finnish = FINNISH1;
	    }
	  }
	  if (finnish == FINNISH2) {
	    for (std::list< std::pair< int, ElementPtr > >::iterator it = nextTokens.begin(); it != nextTokens.end(); ++it) {
	      if (it->first == Parser::LITERAL)
		it->first = Parser::LITERAL2;
	    }
	  }
	  return finnish == FINNISH1 ? Parser::LITERAL : Parser::LITERAL2;
	}
	
    }
  }
}
