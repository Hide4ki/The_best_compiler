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
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	auto myAddEx = new AddEx<T>(_table, this);
	if (myAddEx->derivation(it, end) != EmptyString)
		Add(myAddEx);
	else
	{
		delete myAddEx;
		throw new MyException("Expected:'myAddEx'", place);
	}
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
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
