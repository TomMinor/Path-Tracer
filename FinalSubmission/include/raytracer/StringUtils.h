#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

// Taken from Jon Macey's OBJ importer : http://nccastaff.bournemouth.ac.uk/jmacey/PPP/index.html
void tokenize(const std::string& _str,std::vector<std::string>& _tokens,const std::string& _delimiters);

float tokenToFloat(const std::string& _token);

#endif // STRINGUTILS_H
