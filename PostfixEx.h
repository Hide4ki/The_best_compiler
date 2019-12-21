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
	auto r1 = *it;
	auto token = r1.first;
	auto place = r1.second;
	auto r2 = token.getValue();
	auto name = r2.first;
	auto attribute = r2.second;

	auto myPrimaryEx = new PrimaryEx<T>(_table, this);
	if (myPrimaryEx->derivation(it, end) != EmptyString)
	{
		Add(myPrimaryEx);
	}

	r1 = *it;
	token = r1.first;
	place = r1.second;
	r2 = token.getValue();
	name = r2.first;
	attribute = r2.second;

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
