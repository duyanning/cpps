#include <iostream>
#include "a.y.hpp"

// cpps-macro bison++ a.y

int main()
{
	yy::parser parser;
	return parser.parse();
}
