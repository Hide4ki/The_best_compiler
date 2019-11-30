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
class AssEx : public Program<T>
{
private:
	AssEx() = delete;
public:
	explicit AssEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
AssEx<T>::AssEx(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("AssEx");
}

//<assEx> ::= <id> = <orEx> | ℰ
template<class T>
Program<T>* AssEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::ID)
	{
		++it;
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(_table->getTableID().getName(attribute));
		Add(child);
	}
	else
		throw new MyException("Expected:ID!", place);

	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::ASS_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("=");
		Add(child);
		++it;
	}
	else
		throw new MyException("Expected '='", place);

	auto myOrEx = new OrEx<T>(_table, this);
	if (myOrEx->derivation(it, end) != EmptyString)
		Add(myOrEx);
	else
	{
		delete myOrEx;
		throw new MyException("Expected: r-value", place);
	}
	return this;
}
