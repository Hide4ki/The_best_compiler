#pragma once
#include <string>
#include <exception>
#include <iostream>
#include "Place.h"
#include <vector>
using namespace std;
class Place;

class MyException :exception
{
private:
	Place error_place;
	string what_arg;
	const char* cstr;
public:
	MyException() = delete;
	explicit MyException(string, Place);
	explicit MyException(char *, Place);
	~MyException();
	const char* what()const noexcept;
};
