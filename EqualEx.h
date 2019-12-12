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
class EqualEx : public OrEx<T>
{
private:
	EqualEx() = delete;
public:
	explicit EqualEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
EqualEx<T>::EqualEx(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("EqualEx");
}

//<equalEx> ::= <relEx><equalExAD>
template<class T>
Program<T>* EqualEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myRelEx = new RelEx<T>(_table, this);
	if (myRelEx->derivation(it, end) != EmptyString)
		Add(myRelEx);
	else
	{
		delete myRelEx;
		throw new MyException("Expected:'RelEx'", place);
	}
	bool finish = false;
	auto myEqualExAD = new EqualExAD<T>(_table, this);
	while (!finish)
	{
		if (myEqualExAD->derivation(it, end) != EmptyString)
		{
			Add(myEqualExAD);
			myEqualExAD = new EqualExAD<T>(_table, this);
		}
		else
		{
			delete myEqualExAD;
			finish = true;
		}
	}
	return this;
}
