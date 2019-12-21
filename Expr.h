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

//<expr> ::= <assEx>,<expr> | <assEx> | <orEx>, <expr> | <orEx>, 
template<class T>
Program<T>* Expr<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	bool flag = true;
	auto myAssEx = new AssEx<T>(_table, this);
	if (myAssEx->derivation(it, end) != EmptyString)
		Add(myAssEx);
	else
	{
		delete myAssEx;
		flag = false;
	}
	
	if (!flag)
	{
		return 0;
	}

	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	flag = true;
	if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::COMMA)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(",");
		Add(child);
		flag = false;
		++it;
	}
	if(!flag)
		this->derivation(it, end);
	return this;
}
