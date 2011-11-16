#include "Parser.h"
#include <fstream>

int main () {
  char *filename = "filename";
  std::fstream file (filename, std::fstream::in);
  Parser parser (file);
  std::cerr << "Parsing " << filename << std::endl;
  int result = parser.parse();
  if (result)
    std::cerr << "Parsing was not succesful" << std::endl;
  else
    std::cerr << "Parsing was succesful" << std::endl; 
}