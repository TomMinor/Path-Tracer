#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

//  :

/// @brief Taken from Jon Macey's OBJ importer
/// Modified from :-
/// Jon Macey Tokenize Function [online].
/// [Accessed 2015]. Available from: <http://nccastaff.bournemouth.ac.uk/jmacey/PPP/index.html>
void tokenize(const std::string& _str,std::vector<std::string>& _tokens,const std::string& _delimiters);

///
/// \brief tokenToFloat converts a string to a float
/// \param _token
/// \return
///
float tokenToFloat(const std::string& _token);

#endif // STRINGUTILS_H
