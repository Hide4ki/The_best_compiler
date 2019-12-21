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
class Block : public Program<T>
{
private:
	Block() = delete;
public:
	explicit Block(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
Block<T>::Block(TableSymbol *table, Program<T> *myParent):Program<T>{table,myParent}
{
	SetName("Block");
}

//<block> ::= {<decls><stmts>}
template<class T>
Program<T>* Block<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto r1 = *it;
	auto token = r1.first;
	auto place = r1.second;
	auto r2 = token.getValue();
	auto name = r2.first;
	auto attribute = r2.second;

	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::BRACKETS_L))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("{");
		Add(child);
		++it;
	}
	else
		return 0;

	auto myDecls = new Decls<T>(_table, this);
	myDecls->derivation(it, end);
	delete myDecls;

	auto myStmts = new Stmts<T>(_table, this);
	if (myStmts->derivation(it, end) != EmptyString)
		Add(myStmts);
	else
	{
		delete myStmts;
	}

	r1 = *it;
	token = r1.first;
	place = r1.second;
	r2 = token.getValue();
	name = r2.first;
	attribute = r2.second; 

	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::BRACKETS_R))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("}");
		Add(child);
		++it;
	}
	else
		throw new MyException("Expected:'}'", place);
	return this;
}
