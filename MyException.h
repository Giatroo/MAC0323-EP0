/* Arquivo destinado para a prototipagem da minha própria exception */

#ifndef _MY_EXCEPTION_
#define _MY_EXCEPTION_

#include <iostream>

#include <exception>
#include <string>

/* Essa classe foi feita para fazer o Exception Handling do EP.
 *
 * Ela herda da classe padrão de exceptions do C++ e sobrescreve
 * o método 'what', que retorna uma mensagem dizendo o que aconteceu.
 *
 * Gosto de ter os campos obrigatórios (method e errorType) e um opcional (text).
 *  * O primeiro é do tipo string e é o nome da rotina em que a exception foi
 * lançada.
 *  * O segundo é tipo ErrorType, que contém um código e o um nome de erro, como
 *    ArithmeticException, NullPointerException ...
 *  * O terceiro é do tipo string e é um texto descrevendo o erro
 *
 */
class MyException : public std::exception {
  public: // É preciso declarar a classe antes, para o compilador saber que existe
	class ErrorType {
	  public:
		std::string errorName;
		int errorCode;
		ErrorType(std::string t_errorName, int t_errorCode);
	};

  private:
	ErrorType errorType;
	std::string method;
	std::string text;

  public:
	MyException(ErrorType t_errorType, std::string t_method,
	            std::string t_text = "");

	const char *what() {
		std::string s = "";
		s += this->method;
		s += ": ";
		s += this->errorType.errorName;
		s += " Code ";
		s += std::to_string(this->errorType.errorCode);
		s += "\n";
		s += this->text;
		std::cerr << s.c_str() << (this->text == "" ? "" : "\n");
		return s.c_str();
	}
};

// Os tipos de erro que nós vamos ter
const MyException::ErrorType ErrorTypes[] = {
	{ "NullPointerException", 0 }, { "ArrayIndexOutOfBoundsException", 1 },
	{ "ArithmeticException", 2 },  { "FileNotFoundException", 3 },
	{ "IOException", 4 },          { "EmptyDataStructureException", 5 }
};
// Enum utilizado para acessar o array anterior
enum ErrorCodes {
	NULL_POINTER,
	INDEX_OUT_OF_BOUNDS,
	ARITHMETIC,
	FILE_NOT_FOUND,
	IO,
	EMPTY_DATA_STRUCTURE
};

#endif // !_MY_EXCEPTION_
