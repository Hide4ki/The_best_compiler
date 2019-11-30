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

//<block> ::= <decls><stmts>
template<class T>
Program<T>* Literal<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	return 0;
}
