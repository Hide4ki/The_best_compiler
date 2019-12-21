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
class Stmt : public Program<T>
{
private:
	Stmt() = delete;
public:
	explicit Stmt(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
Stmt<T>::Stmt(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("Stmt");
}

//<stmt> ::= <stmtAD> | <decls><stmtAD>
template<class T>
Program<T>* Stmt<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	bool flag = false;
	auto myDecls = new Decls<T>(_table, this);
	if (myDecls->derivation(it, end) == EmptyString)
	{
		flag = true;
	}
	else
	{
		Add(myDecls);
	}

	auto myStmtAD = new StmtAD<T>(_table, this);
	if (myStmtAD->derivation(it, end) != EmptyString)
		Add(myStmtAD);
	else
	{
		delete myStmtAD;
		if (flag)
			return 0;
	}
	return this;
}
