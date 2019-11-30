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
class Expr : public Program<T>
{
private:
	Expr() = delete;
public:
	explicit Expr(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
Expr<T>::Expr(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("Expr");
}

//<block> ::= <decls><stmts>
template<class T>
Program<T>* Expr<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	return 0;
}
