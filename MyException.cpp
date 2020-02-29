/* Arquivo de implementação da classe MyException declarada no arquivo .h */

#include "MyException.h"

// Construtor da ErrorType
MyException::ErrorType::ErrorType(std::string t_errorName, int t_errorCode)
    : errorName(t_errorName), errorCode(t_errorCode) {}

// Construtor da MyException
MyException::MyException(ErrorType t_errorType, std::string t_method,
                         std::string t_text)
    : errorType(t_errorType), method(t_method), text(t_text) {}
