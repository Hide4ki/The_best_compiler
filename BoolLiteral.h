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
class BoolLiteral : public Program<T>
{
private:
	BoolLiteral() = delete;
public:
	explicit BoolLiteral(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
BoolLiteral<T>::BoolLiteral(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("BoolLiteral");
}

template<class T>
Program<T>* BoolLiteral<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto child = new TerminalSymbol<T>(_table, this);
	if(name == TokenName::WORD && attribute == static_cast<int>(ServWord::FALSE))
		child->givenName("false");
	else if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::FALSE))
		child->givenName("true");
	else 
		throw new MyException("Expected:'true / false'", place);
	Add(child);
	++it;
	return this;
}
