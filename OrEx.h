#pragma once
#include "Program.h"
#include "LexIterator.h"
#include "rules.h"

template <class T>
class Parser;

template <class T>
class LexIterator;

template <class T>
class Program;

template <class T>
class OrEx : public Program<T>
{
private:
	OrEx() = delete;
public:
	explicit OrEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
OrEx<T>::OrEx(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("OrEx");
}

//<orEx> ::= <andEx><orExAD>,
template<class T>
Program<T>* OrEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myAndEx = new AndEx<T>(_table, this);
	if (myAndEx->derivation(it, end) != EmptyString)
		Add(myAndEx);
	else
	{
		delete myAndEx;
		throw new MyException("Expected:'MultiEx'", place);
	}
	auto myOrExAD = new OrExAD<T>(_table, this);
	if (myOrExAD->derivation(it, end) != EmptyString)
		Add(myOrExAD);
	else
	{
		delete myOrExAD;
	}
	return this;
}
