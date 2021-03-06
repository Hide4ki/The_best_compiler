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
class MultiEx : public OrEx<T>
{
private:
	MultiEx() = delete;
public:
	explicit MultiEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
MultiEx<T>::MultiEx(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("MultiEx");
}

//<multiEx> ::= <unaryEx><multiExAD>,
template<class T>
Program<T>* MultiEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myUnaryEx = new UnaryEx<T>(_table, this);
	if (myUnaryEx->derivation(it, end) != EmptyString)
		Add(myUnaryEx);
	else
	{
		delete myUnaryEx;
		throw new MyException("Expected:'unaryEx'", place);
	}
	bool finish = false;
	auto myMultiExAD = new MultiExAD<T>(_table, this);
	while (!finish)
	{
		if (myMultiExAD->derivation(it, end) != EmptyString)
		{
			Add(myMultiExAD);
			myMultiExAD = new MultiExAD<T>(_table, this);
		}
		else
		{
			delete myMultiExAD;
			finish = true;
		}
	}
	return this;
}
