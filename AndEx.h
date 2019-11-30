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
class AndEx : public Program<T>
{
private:
	AndEx() = delete;
public:
	explicit AndEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
AndEx<T>::AndEx(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("AndEx");
}

//<andEx> ::= <equalEx><andExAD>,
template<class T>
Program<T>* AndEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myEqualEx = new EqualEx<T>(_table, this);
	if (myEqualEx->derivation(it, end) != EmptyString)
		Add(myEqualEx);
	else
	{
		delete myEqualEx;
		throw new MyException("Expected:'MultiEx'", place);
	}
	auto myAndExAD = new AndExAD<T>(_table, this);
	if (myAndExAD->derivation(it, end) != EmptyString)
		Add(myAndExAD);
	else
	{
		delete myAndExAD;
	}
	return this;
}
