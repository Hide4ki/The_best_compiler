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
class TerminalSymbol : public Program<T>
{
private:
	TerminalSymbol() = delete;
public:
	void givenName(string name) { SetName(name); };
	explicit TerminalSymbol(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
TerminalSymbol<T>::TerminalSymbol(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
}

template<class T>
Program<T>* TerminalSymbol<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	return this;
}
