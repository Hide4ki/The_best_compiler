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
class AddExAD : public OrEx<T>
{
private:
	AddExAD() = delete;
public:
	explicit AddExAD(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
AddExAD<T>::AddExAD(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("AddExAD");
}

//<addExAD> ::= + <multiEx><addExAD> | - <multiEx><addExAD> | ℰ,
template<class T>
Program<T>* AddExAD<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto r1 = *it;
	auto token = r1.first;
	auto place = r1.second;
	auto r2 = token.getValue();
	auto name = r2.first;
	auto attribute = r2.second;

	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::ADD_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		spush(ExtraType::ADD_OP);
		child->givenName("+");
		Add(child);
		++it;
	}
	else if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::NEG_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("-");
		spush(ExtraType::NEG_OP);
		Add(child);
		++it;
	}
	else
		return 0;

	auto myMultiEx = new MultiEx<T>(_table, this);
	if (myMultiEx->derivation(it, end) != EmptyString)
		Add(myMultiEx);
	else
	{
		delete myMultiEx;
		throw new MyException("Expected:'MultiEx'", place);
	}
	checkop(place);
	return this;
}
