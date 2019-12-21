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
class RelEx : public OrEx<T>
{
private:
	RelEx() = delete;
public:
	explicit RelEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
RelEx<T>::RelEx(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("RelEx");
}

//<relEx>::= <addEx> < <addEx> | <addEx> > <addEx> | <addEx>,
template<class T>
Program<T>* RelEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto r1 = *it;
	auto token = r1.first;
	auto place = r1.second;
	auto r2 = token.getValue();
	auto name = r2.first;
	auto attribute = r2.second;

	auto myAddEx = new AddEx<T>(_table, this);
	if (myAddEx->derivation(it, end) != EmptyString)
		Add(myAddEx);
	else
	{
		delete myAddEx;
		throw new MyException("Expected:'myAddEx'", place);
	}

	r1 = *it;
	token = r1.first;
	place = r1.second;
	r2 = token.getValue();
	name = r2.first;
	attribute = r2.second;

	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::LESS_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("<");
		spush(ExtraType::LESS_OP);
		Add(child);
		++it;
		auto myAddEx = new AddEx<T>(_table, this);
		if (myAddEx->derivation(it, end) != EmptyString)
			Add(myAddEx);
		else
		{
			delete myAddEx;
			throw new MyException("Expected:'AddEx'", place);
		}
		checkop(place);
	}
	else if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::LARGE_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(">");
		spush(ExtraType::LARGE_OP);
		Add(child);
		++it;
		auto myAddEx = new AddEx<T>(_table, this);
		if (myAddEx->derivation(it, end) != EmptyString)
			Add(myAddEx);
		else
		{
			delete myAddEx;
			throw new MyException("Expected:'AddEx'", place);
		}
		checkop(place);
	}
	return this;
}
