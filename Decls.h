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
class Decls : public Program<T>
{
private:
	Decls() = delete;
public:
	explicit Decls(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
Decls<T>::Decls(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("Decls");
}

//<decls> :: = <decl><decls> | ℰ,
template<class T>
Program<T>* Decls<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto myDecl = new Decl<T>(_table, this);
	if (myDecl->derivation(it, end) != EmptyString)
		Add(myDecl);
	else
	{
		delete myDecl;
		return 0;
	}
	this->derivation(it, end);
	return this;
}
