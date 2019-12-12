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
class MultiExAD : public OrEx<T>
{
private:
	MultiExAD() = delete;
public:
	explicit MultiExAD(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
MultiExAD<T>::MultiExAD(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("MultiExAD");
}

//<multiExAD> ::= * <unaryEx><multiExAD> | / <unaryEx><multiExAD> | ℰ,
template<class T>
Program<T>* MultiExAD<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::DIV_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("/");
		spush(ExtraType::DIV_OP);
		Add(child);
		++it;
	}
	else if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::MUL_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("*");
		spush(ExtraType::MUL_OP);
		Add(child);
		++it;
	}
	else
		return 0;

	auto myUnaryEx = new UnaryEx<T>(_table, this);
	if (myUnaryEx->derivation(it, end) != EmptyString)
		Add(myUnaryEx);
	else
	{
		delete myUnaryEx;
		throw new MyException("Expected:'unaryEx'", place);
	}
	checkop(place);
	return this;
}
