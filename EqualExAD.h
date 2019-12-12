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
class EqualExAD : public OrEx<T>
{
private:
	EqualExAD() = delete;
public:
	explicit EqualExAD(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
EqualExAD<T>::EqualExAD(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("EqualExAD");
}

//<equalExAD> ::= == <relEx><equalExAD> | != <relEx><equalExAD> | ℰ,
template<class T>
Program<T>* EqualExAD<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::EQ_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("==");
		spush(ExtraType::EQ_OP);
		Add(child);
		++it;
	}
	else if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::NOTEQ_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("!=");
		spush(ExtraType::NOTEQ_OP);
		Add(child);
		++it;
	}
	else
		return 0;

	auto myRelEx = new RelEx<T>(_table, this);
	if (myRelEx->derivation(it, end) != EmptyString)
		Add(myRelEx);
	else
	{
		delete myRelEx;
		throw new MyException("Expected:'RelEx'", place);
	}
	checkop(place);
	return this;
}
