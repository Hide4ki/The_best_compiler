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
class AddEx : public Program<T>
{
private:
	AddEx() = delete;
public:
	explicit AddEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
AddEx<T>::AddEx(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("AddEx");
}

//<addEx>::= <multiEx><addExAD>,
template<class T>
Program<T>* AddEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myMultiEx = new MultiEx<T>(_table, this);
	if (myMultiEx->derivation(it, end) != EmptyString)
		Add(myMultiEx);
	else
	{
		delete myMultiEx;
		throw new MyException("Expected:'MultiEx'", place);
	}
	auto myAddExAD = new AddExAD<T>(_table, this);
	if (myAddExAD->derivation(it, end) != EmptyString)
		Add(myAddExAD);
	else
	{
		delete myAddExAD;
	}
	return this;
}
