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
class UnaryEx : public Program<T>
{
private:
	UnaryEx() = delete;
public:
	explicit UnaryEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
UnaryEx<T>::UnaryEx(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("UnaryEx");
}

//<unaryEx> ::=  ++<postfixEx> | -<postfixEx> | <postfixEx>,
template<class T>
Program<T>* UnaryEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::NEG_OP)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("-");
		Add(child);
		++it;
	} 
	else if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::INC_OP)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("++");
		Add(child);
		++it;
	}
	auto myPostfixEx = new PostfixEx<T>(_table, this);
	if (myPostfixEx->derivation(it, end) != EmptyString)
	{
		Add(myPostfixEx);
	}
	return this;
}
