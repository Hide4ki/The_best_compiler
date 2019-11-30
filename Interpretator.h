#pragma once
#include "Scanner.h"
#include "TableSymbol.h"
#include "Parser.h"
#include "LexIterator.h"

using namespace std;

template <class T>
class Parser;

template<class T>
class Interpretator
{
private:
	Parser<T> *_myParser;
	TableSymbol *_myTableSymbol;
public:
	void execute();
	Interpretator(T, T);
	~Interpretator();
};

template<class T>
void Interpretator<T>::execute()
{
	_myParser->getProgram();
	//_myProgram = _myParser->getProgram();
	//_myProgram = _myOptimizer(_myProgram->begin(), _myProgram->end());
	//_myCode = _myGeneratorCode(_myProgram->begin(), _myProgram->end());
	//_myCode->execute();
	cout << _myTableSymbol->getTableDelim() << endl;
	cout << _myTableSymbol->getTableID() << endl;
	cout << _myTableSymbol->getTableServWord() << endl;
	cout << _myTableSymbol->getTableLiterals() << endl;
}

template<class T>
Interpretator<T>::Interpretator(T it, T end)
{
	_myTableSymbol = new TableSymbol;
	Scanner<T> *myScanner = new Scanner<T>(it, end);
	myScanner->SetTable(_myTableSymbol);
	myScanner->nextToken();
	_myParser = new Parser<T>(LexIterator<T>{ myScanner }, LexIterator<T>{ new Scanner<T>{} });
	_myParser->SetTable(_myTableSymbol);
}

template<class T>
Interpretator<T>::~Interpretator()
{
	delete _myParser;
	delete _myTableSymbol;
}
