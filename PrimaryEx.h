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
class PrimaryEx : public Program<T>
{
private:
	PrimaryEx() = delete;
public:
	explicit PrimaryEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
PrimaryEx<T>::PrimaryEx(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("PrimaryEx");
}

//<primaryEx> ::= (<orEx>) | <id> | <literal>
template<class T>
Program<T>* PrimaryEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	
	if (name == TokenName::ID)
	{
		checkdecl(attribute,place);
		++it;
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(_table->getTableID().getName(attribute));
		Add(child);
		return this;
	}

	auto myLiteral = new Literal<T>(_table, this);
	if (myLiteral->derivation(it, end) != EmptyString)
	{
		Add(myLiteral);
		return this;
	}
	else
	{
		delete myLiteral;
	}

	if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::RBRACKETS_L)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("(");
		Add(child);
		++it;
	}
	else
		throw new MyException("Expected:'('", place);

	auto myOrEx = new OrEx<T>(_table, this);
	if (myOrEx->derivation(it, end) != EmptyString)
		Add(myOrEx);
	else
	{
		delete myOrEx;
		throw new MyException("Expected: expression", place);
	}

	if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::RBRACKETS_R)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(")");
		Add(child);
		++it;
	}
	else
		throw new MyException("Expected:')'", place);
	return this;
}
