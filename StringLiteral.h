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
class StringLiteral : public Program<T>
{
private:
	StringLiteral() = delete;
public:
	explicit StringLiteral(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
StringLiteral<T>::StringLiteral(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("StringLiteral");
}

template<class T>
Program<T>* StringLiteral<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto child = new TerminalSymbol<T>(_table, this);
	child->givenName(_table->getTableLiterals().getStr(attribute));
	Add(child);
	spush(ExtraType::STR);
	++it;
	return this;
}
