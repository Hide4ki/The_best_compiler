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
class UnaryEx : public OrEx<T>
{
private:
	UnaryEx() = delete;
public:
	explicit UnaryEx(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
UnaryEx<T>::UnaryEx(TableSymbol *table, Program<T> *myParent) :OrEx<T>{ table,myParent }
{
	SetName("UnaryEx");
}

//<unaryEx> ::=  ++<postfixEx> | -<postfixEx> | <postfixEx>,
template<class T>
Program<T>* UnaryEx<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::NEG_OP)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("-");
		Add(child);
		++it;
	} 	
	else if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::NOT_OP)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("!");
		Add(child);
		++it; 
	}
	else if ((name == TokenName::DELIM) && (attribute == static_cast<int>(Delim::INC_OP)))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("++");
		Add(child);
		++it; 
	}
	auto myPostfixEx = new PostfixEx<T>(_table, this);
	if (myPostfixEx->derivation(it, end) != EmptyString)
	{
		Add(myPostfixEx);
	}
	if (attribute == static_cast<int>(Delim::NOT_OP))
		checknot(place);
	if (attribute == static_cast<int>(Delim::INC_OP) || attribute == static_cast<int>(Delim::NEG_OP))
	{
		checkunary(place);
		if (attribute == static_cast<int>(Delim::NEG_OP))
			put_lex(make_op(ExtraType::NEGU_OP));
		if (attribute == static_cast<int>(Delim::INC_OP))
			put_lex5(make_op(ExtraType::INC_OP));

		
	}
	return this;
}
