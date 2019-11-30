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
	auto [token, place] = *it;
	auto [name, attribute] = token.getValue();
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
	if (myDecls->derivation(it, end) != EmptyString)
		Add(myDecls);
	else
	{
		delete myDecls;
	}

	auto myStmts = new Stmts<T>(_table, this);
	if (myStmts->derivation(it, end) != EmptyString)
		Add(myStmts);
	else
	{
		delete myStmts;
	}

	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
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
