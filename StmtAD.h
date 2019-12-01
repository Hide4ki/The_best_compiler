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
class StmtAD : public Program<T>
{
private:
	StmtAD() = delete;
public:
	explicit StmtAD(TableSymbol*, Program<T>*);
	Program<T> *derivation(LexIterator<T>&, LexIterator<T>&) override;
	friend class Parser<T>;
};

template<class T>
StmtAD<T>::StmtAD(TableSymbol *table, Program<T> *myParent) :Program<T>{ table,myParent }
{
	SetName("StmtAD");
}

//<stmtAD> ::= <assEx>; | <block> | for(<assEx>; <orEx>; <expr>) <stmt>  endfor; | <expr>; | while(<orEx>) <stmt> endwhile; | if (<orEx>) <stmt> else <stmt> endif; | if (<orEx>) <stmt> endif | ℰ;
template<class T>
Program<T>* StmtAD<T>::derivation(LexIterator<T>&it, LexIterator<T>&end)
{
	auto[token, place] = *it;
	auto[name, attribute] = token.getValue();
	//for(<assEx>; <orEx>; <expr>) <stmt>  endfor;
	if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::FOR))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("for");
		Add(child);
		++it;

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();

		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_L))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("(");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'('", place);

		auto myAssEx = new AssEx<T>(_table, this);
		if (myAssEx->derivation(it, end) != EmptyString)
			Add(myAssEx);
		else
		{
			delete myAssEx;
		}

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
			throw new MyException("Expected:';'", place);

		auto myOrEx = new OrEx<T>(_table, this);
		if (myOrEx->derivation(it, end) != EmptyString)
			Add(myOrEx);
		else
		{
			delete myOrEx;
			throw new MyException("Expected: condition", place);
		}

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
			throw new MyException("Expected:';'", place);

		auto myExpr = new Expr<T>(_table, this);
		if (myExpr->derivation(it, end) != EmptyString)
			Add(myExpr);
		else
		{
			delete myExpr;
		}

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_R))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(")");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:')'", place);

		auto myStmt = new Stmt<T>(_table, this);
		if (myStmt->derivation(it, end) != EmptyString)
			Add(myStmt);
		else
		{
			delete myStmt;
		}

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ENDFOR))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("ENDFOR");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'endfor'", place);
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
			throw new MyException("Expected:';'", place);
	}
	else if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::WHILE))
	{	//while (<orEx>) <stmt> endwhile;
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("while");
		Add(child);
		++it;

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();

		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_L))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("(");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'('", place);

		auto myOrEx = new OrEx<T>(_table, this);
		if (myOrEx->derivation(it, end) != EmptyString)
			Add(myOrEx);
		else
		{
			delete myOrEx;
			throw new MyException("Expected: condition", place);
		}

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_R))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(")");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:')'", place);

		auto myStmt = new Stmt<T>(_table, this);
		if (myStmt->derivation(it, end) != EmptyString)
			Add(myStmt);
		else
		{
			delete myStmt;
		}

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ENDWHILE))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("endwhile");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'endwhile'", place);
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
			throw new MyException("Expected:';'", place);
	}
	else if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::IF))
	{//if (<orEx>) <stmt> else <stmt> endif; | if (<orEx>) <stmt> endif
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("if");
		Add(child);
		++it;

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();

		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_L))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("(");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'('", place);

		auto myOrEx = new OrEx<T>(_table, this);
		if (myOrEx->derivation(it, end) != EmptyString)
			Add(myOrEx);
		else
		{
			delete myOrEx;
			throw new MyException("Expected: condition", place);
		}

		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_R))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(")");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:')'", place);

		auto myStmt = new Stmt<T>(_table, this);
		if (myStmt->derivation(it, end) != EmptyString)
			Add(myStmt);
		else
		{
			delete myStmt;
		}
		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ELSE))
		{
			++it;
			auto myStmt = new Stmt<T>(_table, this);
			if (myStmt->derivation(it, end) != EmptyString)
				Add(myStmt);
			else
			{
				delete myStmt;
			}
		}
		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ENDIF))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("endif");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'endif'", place);
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
			throw new MyException("Expected:';'", place);
	}
	else
	{
		bool derivationIdentified = true;
		auto myAssEx = new AssEx<T>(_table, this);
		if (myAssEx->derivation(it, end) != EmptyString)
			Add(myAssEx);
		else
		{
			derivationIdentified = false;
			delete myAssEx;
		}

		if (!derivationIdentified)
		{
			auto myExpr = new Expr<T>(_table, this);
			if (myExpr->derivation(it, end) != EmptyString)
				Add(myExpr);
			else
			{
				derivationIdentified = false;
				delete myExpr;
			}
		}
		auto[token, place] = *it;
		auto[name, attribute] = token.getValue();
		if (derivationIdentified)
			if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::SEMICOLON))
			{
				auto child = new TerminalSymbol<T>(_table, this);
				child->givenName(";");
				Add(child);
				++it;
			}
			else
				throw new MyException("Expected:';'", place);

		if (!derivationIdentified)
		{
			auto myBlock = new Block<T>(_table, this);
			if (myBlock->derivation(it, end) != EmptyString)
			{
				derivationIdentified = true;
				Add(myBlock);
			}
			else
			{
				derivationIdentified = false;
				delete myBlock;
			}
		}

		if (!derivationIdentified)
			return 0;
	}
	return this;
}
