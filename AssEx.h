#pragma once
#include "Program.h"
#include "LexIterator.h"
#include "rules.h"

template <class T>
class Parser;

template <class T>
class LexIterator;

template <class T>
class OrEx;

template <class T>
class AssEx : public OrEx<T>
{
private:
	AssEx() = delete;
public:
	explicit AssEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
AssEx<T>::AssEx(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("AssEx");
}

//<assEx> ::= <id> = <orEx> | ℰ
template<class T>
Program<T>* AssEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto r1 = *it;
	auto token = r1.first;
	auto place = r1.second;
	auto r2 = token.getValue();
	auto name = r2.first;
	auto attribute = r2.second;

	if (name == TokenName::ID)
	{
		++it;
		checkdecl(attribute, place);
		put_lex5(token);
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(_table->getTableID().getName(attribute));
		Add(child);
	}
	else
		return 0;

	r1 = *it;
	token = r1.first;
	place = r1.second;
	r2 = token.getValue();
	name = r2.first;
	attribute = r2.second;

	if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::ASS_OP))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("=");

		Add(child);
		++it;
	}
	else
		throw new MyException("Expected '='", place);

	auto myOrEx = new OrEx<T>(_table, this);
	if (myOrEx->derivation(it, end) != EmptyString)
		Add(myOrEx);
	else
	{
		delete myOrEx;
		throw new MyException("Expected: r-value", place);
	}
	eqtype(place);
	put_lex(make_op(ExtraType::ASS_OP));
	return this;
}
