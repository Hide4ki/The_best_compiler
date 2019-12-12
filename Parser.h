#pragma once
#include <utility>
#include "Interpretator.h"
#include "TableSymbol.h"
#include "Scanner.h"
#include "LexIterator.h"
#include "Program.h"
#include "Rules.h"

using namespace std;

class TableSymbol;

template <class T>
class Scanner;

template <class T>
class Interpretator;

template <class T>
class LexIterator;

template <class T>
class Parser
{
private:
	LexIterator<T> _it;
	LexIterator<T> _end;
	Parser() = delete;
	Parser(LexIterator<T>&, LexIterator<T>&);
	~Parser();
	TableSymbol *_table;
	void SetTable(TableSymbol *);
	Program<T> *getProgram();
public:
	friend Interpretator<T>;
};

template<class T>
Parser<T>::Parser(LexIterator<T>&it, LexIterator<T>&end) :_it{ move(it) }, _end{ move(end) }
{
}

template<class T>
Parser<T>::~Parser()
{
}

template<class T>
void Parser<T>::SetTable(TableSymbol *table)
{
	_table = table;
}

template<class T>
Program<T> * Parser<T>::getProgram()
{
	auto myProgram = new Program<T>(_table);
	try
	{
		myProgram->derivation(_it, _end);
	}
	catch (MyException *e)
	{
		cout << e->what() << endl;
		delete e;
	}
	return myProgram;
}
