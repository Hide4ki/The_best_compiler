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
class Literal : public Program<T>
{
private:
	Literal() = delete;
public:
	explicit Literal(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
Literal<T>::Literal(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("Literal");
}

//<literal> ::= <numLiteral> | <strLiteral> | <boolLiteral
template<class T>
Program<T>* Literal<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::LITERAL)
	{
		auto type = _table->getTableLiterals().getType(attribute);
		if (type == Type::INTEGER)
		{
			auto myIntegerLiteral = new IntegerLiteral<T>(_table, this);
			if (myIntegerLiteral->derivation(it, end) != EmptyString)
				Add(myIntegerLiteral);
			else
			{
				delete myIntegerLiteral;
			}
		}
		else if (type == Type::STRING)
		{
			auto myStringLiteral = new StringLiteral<T>(_table, this);
			if (myStringLiteral->derivation(it, end) != EmptyString)
				Add(myStringLiteral);
			else
			{
				delete myStringLiteral;
			}
		}
	}
	else
	{
		auto myBoolLiteral = new BoolLiteral<T>(_table, this);
		if (myBoolLiteral->derivation(it, end) != EmptyString)
			Add(myBoolLiteral);
		else
		{
			delete myBoolLiteral;
		}
	}
	return this;
}
