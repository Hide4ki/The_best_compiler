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
class OrEx : public Program<T> 
{
private:
	OrEx() = delete;
protected:
public:
	explicit OrEx(TableSymbol*, Program<T>*);
	void ConvertToSyntaxTree() override;
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
OrEx<T>::OrEx(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("OrEx");
}

template<class T>
inline void OrEx<T>::ConvertToSyntaxTree()
{
	Program<T>::ConvertToSyntaxTree();
	auto[child, end1] = CreateIterator();
	for(; child != end1; child++)
		if ((*child)->GetNumChilds() == 1)
		{
			auto[it, end2] = (*child)->CreateIterator();
			auto tmp = *it;
			(*child)->Remove(tmp);
			delete *child;
			*child = tmp;
			(tmp->_parent) = this;
		}
}

//<orEx> ::= <andEx><orExAD>,
template<class T>
Program<T>* OrEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myAndEx = new AndEx<T>(_table, this);
	if (myAndEx->derivation(it, end) != EmptyString)
		Add(myAndEx);
	else
	{
		delete myAndEx;
		throw new MyException("Expected:'MultiEx'", place);
	}
	bool finish = false;
	auto myOrExAD = new OrExAD<T>(_table, this);
	while (!finish)
	{
		if (myOrExAD->derivation(it, end) != EmptyString)
		{
			Add(myOrExAD);
			myOrExAD = new OrExAD<T>(_table, this);
		}
		else
		{
			delete myOrExAD;
			finish = true;
		}
	}
	return this;
}
