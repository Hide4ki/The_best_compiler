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
class OrExAD : public Program<T>
{
private:
	OrExAD() = delete;
public:
	explicit OrExAD(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
OrExAD<T>::OrExAD(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("OrExAD");
}

//<orExAD> ::= ||<andEx><orExAD> | ℰ,
template<class T>
Program<T>* OrExAD<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::OR_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("||");
		Add(child);
		++it;
	}
	else
		return 0;

	auto myAndEx = new AndEx<T>(_table, this);
	if (myAndEx->derivation(it, end) != EmptyString)
		Add(myAndEx);
	else
	{
		delete myAndEx;
		throw new MyException("Expected:'AndEx'", place);
	}
	this->derivation(it, end);
	return this;
}
