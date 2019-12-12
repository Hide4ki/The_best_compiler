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
class IntegerLiteral : public Program<T>
{
private:
	IntegerLiteral() = delete;
public:
	explicit IntegerLiteral(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
IntegerLiteral<T>::IntegerLiteral(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("IntegerLiteral");
}

template<class T>
Program<T>* IntegerLiteral<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto child = new TerminalSymbol<T>(_table, this);
	child->givenName(to_string(_table->getTableLiterals().getNum(attribute)));
	Add(child);
	spush(ExtraType::INT);
	++it;
	return this;
}
