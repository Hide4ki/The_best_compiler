#pragma once
#include <vector>
#include <iterator>
#include <utility>
#include <string>
#include "LexIterator.h"
#include "Parser.h"
#include "TableSymbol.h"
#include "Token.h"
#include "Place.h"
#include "rules.h"

using namespace std;

template <class T>
class LexIterator;

template <class T>
class Parser;

template <class T>
class Program
{
private:
	string _name{ "Program" };
	Program() = delete;
protected:
	void decid(int, Type, Place place);
	explicit Program(TableSymbol*, Program<T>* myParent = 0);
	void SetName(string name) { _name = name; };
	Program<T>* _parent;
	TableSymbol *_table;
	vector<Program<T>*> _childs;
public:
	virtual ~Program();
	virtual Program<T> *derivation(LexIterator<T>&,LexIterator<T>&);
	string Name() { return _name; };
	void Add(Program<T>*);
	void Remove(Program<T>*);
	Program<T>* GetParent();
	pair<typename vector<Program<T>*>::iterator, typename vector<Program<T>*>::iterator> CreateIterator();
	friend class Parser<T>;
};

template<class T>
void Program<T>::decid(int i, Type type, Place place)
{
	if (!_table->getTableID().setType(i, type))
		throw new MyException("Repeated declare!!!", place);
}

template<class T>
Program<T>::Program(TableSymbol*table, Program<T>* myParent) :_parent{ myParent }, _table{ table }
{
}

template<class T>
Program<T>::~Program()
{
	for (auto it : _childs)
		delete it;
}

//<prog> ::= PROGRAMM;<decls><stmts>END;
template<class T>
Program<T>* Program<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();

	if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::PROGRAMM))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("PROGRAMM");
		Add(child);
		++it;
	}
	else
		throw new MyException("Expected:Service Word PROGRAMM!", place);

	auto [token, place] = *it;
	auto [name, attribute] = token.getValue();

	if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::SEMICOLON)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(";");
		Add(child);
		++it;
	}
	else 
		throw new MyException("Expected:';'", place);

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
	if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::END))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("END");
		Add(child);
		++it;
	}
	else
		throw new MyException("Expected:Service Word END!", place);

	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::SEMICOLON))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(";");
		Add(child);
		++it;
	}
	else
		throw new MyException("Expected ';'", place);

	if (it != end)
		throw new MyException("Expected: Finish program!", place);
	else
		throw new MyException("ALL OK!!", place);
	return this;
}

template<class T>
void Program<T>::Add(Program<T>*child)
{
	_childs.push_back(child);
}

template<class T>
void Program<T>::Remove(Program<T>*child)
{
	auto tmp = find(_childs.begin(), _childs.end(), child);
	if (tmp != _childs.end())
	{
		delete *tmp;
		*tmp = move(_childs.back());
		_childs.pop_back();
	}
}

template<class T>
Program<T> * Program<T>::GetParent()
{
	return _parent;
}

template<class T>
typename pair<typename vector<Program<T>*>::iterator, typename vector<Program<T>*>::iterator> Program<T>::CreateIterator()
{
	return pair<typename vector<Program<T>*>::iterator, typename vector<Program<T>*>::iterator>{_childs.begin(), _childs.end()};
}
