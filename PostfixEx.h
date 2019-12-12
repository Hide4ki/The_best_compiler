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
class PostfixEx : public OrEx<T>
{
private:
	PostfixEx() = delete;
public:
	explicit PostfixEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
PostfixEx<T>::PostfixEx(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("PostfixEx");
}

//<postfixEx> ::= <primaryEx>++ | <primaryEx>,
template<class T>
Program<T>* PostfixEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myPrimaryEx = new PrimaryEx<T>(_table, this);
	if (myPrimaryEx->derivation(it, end) != EmptyString)
	{
		Add(myPrimaryEx);
	}

	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::INC_OP)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("++");
		checkunary(place);
		Add(child);
		++it;
	}
	return this;
}
