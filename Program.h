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
#include <stack>

using namespace std;

template <class T>
class LexIterator;

template <class T>
class Parser;
enum class ExtraType{ INT, STR, BOOL, OR_OP, AND_OP, EQ_OP, NOTEQ_OP, ASS_OP, LARGE_OP, LESS_OP, INC_OP, ADD_OP, DIV_OP, MUL_OP, NOT_OP, NEG_OP, WRONGTYPE};
template <class T>
class Program
{
private:
	Program() = delete;
	string _name{ "Program" };
	int _id;
	Program<T>* _parent;
	vector<Program<T>*> _childs;
	static stack<ExtraType> _checkOperator;
protected:
	void decid(int, Type, Place place);
	void checkdecl(int, Place place);
	ExtraType getType(ExtraType, ExtraType, ExtraType);
	void spush(ExtraType);
	ExtraType spop();
	void checkop(Place);
	void checknot(Place);
	void checkunary(Place);
	void eqtype(Place);
	void eqbool(Place);
	explicit Program(TableSymbol*, Program<T>* myParent = 0);
	void SetName(string name) { _name = name; };
	TableSymbol *_table;
public:
	int GetNumChilds() { return _childs.size(); };
	virtual ~Program();
	virtual Program<T> *derivation(LexIterator<T>&,LexIterator<T>&);
	string Name() { return _name; };
	virtual void Add(Program<T>*);
	void Remove(Program<T>*);
	Program<T>* GetParent();
	void PrintTree(ofstream &);
	pair<typename vector<Program<T>*>::iterator, typename vector<Program<T>*>::iterator> CreateIterator();
	friend class Parser<T>;
};
template <class T>
stack<ExtraType> Program<T>::_checkOperator;

template<class T>
void Program<T>::decid(int i, Type type, Place place)
{
	if (!_table->getTableID().setType(i, type))
		throw new MyException("Repeated declare!!!", place);
}

template<class T>
void Program<T>::checkdecl(int i, Place place)
{
	if (!_table->getTableID().declare(i))
		throw new MyException("Don't declare!!!", place);
	auto type = (_table->getTableID()).getType(i);
	switch (type)
	{
	case Type::INTEGER:
		spush(ExtraType::INT);
		break;
	case Type::BOOL:
		spush(ExtraType::BOOL);
		break;
	case Type::STRING:
		spush(ExtraType::STR);
		break;
	}
}

template<class T>
inline ExtraType Program<T>::getType(ExtraType op, ExtraType t1, ExtraType t2)
{//bool str int
	switch (op)
	{
	case ExtraType::AND_OP:
	case ExtraType::OR_OP:
		if (t1 == ExtraType::BOOL && t2 == ExtraType::BOOL)
			return ExtraType::BOOL;
		else
			return ExtraType::WRONGTYPE;
		break;
	case ExtraType::EQ_OP:
	case ExtraType::NOTEQ_OP:
		if (t1 == ExtraType::BOOL && t2 == ExtraType::BOOL)
			return ExtraType::BOOL;
		else if(t1 == ExtraType::INT && t2 == ExtraType::INT)
			return ExtraType::BOOL;
		else if (t1 == ExtraType::STR && t2 == ExtraType::STR)
			return ExtraType::BOOL;
		else
			return ExtraType::WRONGTYPE;
		break;
	case ExtraType::LARGE_OP:
	case ExtraType::LESS_OP:
		if (t1 == ExtraType::INT && t2 == ExtraType::INT)
			return ExtraType::BOOL;
		else
			return ExtraType::WRONGTYPE;
		break;
	case ExtraType::NEG_OP:
	case ExtraType::ADD_OP:
	case ExtraType::DIV_OP:
	case ExtraType::MUL_OP:
		if (t1 == ExtraType::INT && t2 == ExtraType::INT)
			return ExtraType::INT;
		else
			return ExtraType::WRONGTYPE;
		break;
	}
}

template<class T>
inline void Program<T>::spush(ExtraType nType)
{
	_checkOperator.push(nType);
}

template<class T>
inline ExtraType Program<T>::spop()
{
	auto tmp = _checkOperator.top();
	_checkOperator.pop();
	return tmp;
}

template<class T>
inline void Program<T>::checkop(Place here)
{
	auto t2 = spop();
	auto op = spop();
	auto t1 = spop();
	auto res = getType(op, t1, t2);
	if (res != ExtraType::WRONGTYPE)
		spush(res);
	else
		throw new MyException("Wrong type, another type was expected!!!", here);
}

template<class T>
inline void Program<T>::checknot(Place here)
{
	if (spop() != ExtraType::BOOL)
		throw new MyException("\'!\' applicade only bool type", here);
	else
		spush(ExtraType::BOOL);
}

template<class T>
inline void Program<T>::checkunary(Place here)
{
	if (spop() != ExtraType::INT)
		throw new MyException("unary operator applicade only integer type", here);
	else
		spush(ExtraType::INT);
}

template<class T>
inline void Program<T>::eqtype(Place here)
{
	if (spop() != spop())
		throw new MyException("Assigned operator applicade only equals type", here);
}

template<class T>
inline void Program<T>::eqbool(Place here)
{
	if (spop() != ExtraType::BOOL)
		throw new MyException("Controls statements use for condition only  bool type!!!", here);
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
	static int id = 0;
	child->_id = id++;
	_childs.push_back(child);
}

template<class T>
void Program<T>::Remove(Program<T>*child)
{
	auto tmp = find(_childs.begin(), _childs.end(), child);
	if (tmp != _childs.end())
	{
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
inline void Program<T>::PrintTree(ofstream &stream)
{
	for (auto &ch : _childs)
	{
		stream << "\t\"" << "nodeId = " << _id <<  " | "  << _name << "\"->\"" << "nodeId = " << ch->_id << " | " << ch->_name  << "\";" << endl;
		ch->PrintTree(stream); 
	}
}

template<class T>
typename pair<typename vector<Program<T>*>::iterator, typename vector<Program<T>*>::iterator> Program<T>::CreateIterator()
{
	return pair<typename vector<Program<T>*>::iterator, typename vector<Program<T>*>::iterator>{_childs.begin(), _childs.end()};
}
