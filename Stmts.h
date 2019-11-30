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
class Stmts : public Program<T>
{
private:
	Stmts() = delete;
public:
	explicit Stmts(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
Stmts<T>::Stmts(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("Stmts");
}

//<stmts> ::= <stmt><stmts> | ℰ, 
template<class T>
Program<T>* Stmts<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto myStmt = new Stmt<T>(_table, this);
	if (myStmt->derivation(it, end) != EmptyString)
		Add(myStmt);
	else
	{
		delete myStmt;
		return 0;
	}
	this->derivation(it, end);
	return this;
}


