#include "MyException.h"
#include <vector>
#include <string>

using namespace std;

MyException::MyException(string message, Place place_code) :what_arg{ message }, error_place{ place_code }
{
	auto[a, b] = error_place.getPlace();
	auto str = (message + " " + to_string(a) + "|" + to_string(b)).c_str();
	cstr = _strdup(str);
}

MyException::MyException(char * message, Place place_code) : what_arg{ message }, error_place{ place_code }
{
	auto[a, b] = error_place.getPlace();
	auto str = (string(message) += string(" ") += string(to_string(a)) += string("|") += string(to_string(b)));
	cstr = _strdup(str.c_str());
}

MyException::~MyException()
{
	delete cstr;
}

const char* MyException::what()const noexcept
{
	return cstr;
}