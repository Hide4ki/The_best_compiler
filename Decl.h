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
class Decl : public Program<T>
{
private:
	Decl() = delete;
public:
	explicit Decl(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
Decl<T>::Decl(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("Decl");
}

//<decl> :: = <type><id>;
template<class T>
Program<T>* Decl<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	Type typeDecl;
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::WORD && (attribute == static_cast<int>(ServWord::BOOL) || attribute == static_cast<int>(ServWord::INTEGER) || attribute == static_cast<int>(ServWord::STRING)))
	{
		++it;
		auto child = new TerminalSymbol<T>(_table, this);
		switch (attribute)
		{
		case ServWord::BOOL:
			typeDecl = Type::BOOL;
			child->givenName("bool");
			Add(child);
			break;
		case ServWord::INTEGER:
			typeDecl = Type::INTEGER;
			child->givenName("integer");
			Add(child);
			break;
		case ServWord::STRING:
			typeDecl = Type::STRING; 
			child->givenName("string");
			Add(child);
			break;
		}
	}
	else
		return 0;
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	if (name == TokenName::ID)
	{
		++it;
		decid(attribute, typeDecl, place);
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName(_table->getTableID().getName(attribute));
		Add(child);
	}
	else
		throw new MyException("Expected:ID!", place);

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

	return this;
}
