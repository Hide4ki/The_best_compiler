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
class AndExAD : public Program<T>
{
private:
	AndExAD() = delete;
public:
	explicit AndExAD(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
AndExAD<T>::AndExAD(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("AndExAD");
}

//<andExAD> ::= && <equalEx><andExAD> | ℰ,
template<class T>
Program<T>* AndExAD<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::AND_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("&&");
		Add(child);
		++it;
	}
	else
		return 0;

	auto myEqualEx = new EqualEx<T>(_table, this);
	if (myEqualEx->derivation(it, end) != EmptyString)
		Add(myEqualEx);
	else
	{
		delete myEqualEx;
		throw new MyException("Expected:'EqualEx'", place);
	}
	this->derivation(it, end);
	return this;
}
