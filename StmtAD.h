#pragma once
#include "Program.h"
#include "LexIterator.h"
#include "rules.h"
#include "Token.h"

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
	int pl1;
	int pl2;
	int pl3;
	int pl4;
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
	auto r1 = *it;
	auto token = r1.first;
	auto place = r1.second;
	auto r2 = token.getValue();
	auto name = r2.first;
	auto attribute = r2.second;
	if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::WRITE))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("write");
		Add(child);
		++it;

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		auto myOrEx = new OrEx<T>(_table, this);
		if (myOrEx->derivation(it, end) != EmptyString)
			Add(myOrEx);
		else
		{
			delete myOrEx;
			throw new MyException("Expected: expreshion", place);
		}
		put_lex(make_op(ExtraType::WRITE_OP));
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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
	else if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::READ))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("read");
		Add(child);
		++it;

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		if (name == TokenName::ID)
		{
			checkdecl(attribute, place);
			put_lex5(token);
			++it;
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(_table->getTableID().getName(attribute));
			Add(child);
		}
		else 
			throw new MyException("Expected: id", place);
		put_lex(make_op(ExtraType::READ_OP));
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::SEMICOLON))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(";");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:';'", place);
	}//for(<assEx>; <orEx>; <expr>) <stmts>  endfor;
	else if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::FOR))
	{
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("for");
		Add(child);
		++it;

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::SEMICOLON))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(";");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:';'", place);

		pl2 = _polis.size();
		auto myOrEx = new OrEx<T>(_table, this);
		if (myOrEx->derivation(it, end) != EmptyString)
			Add(myOrEx);
		else
		{
			delete myOrEx;
			throw new MyException("Expected: condition", place);
		}
		eqbool(place);

		pl3 = _polis.size();
		put_lex(make_op(ExtraType::WRONGTYPE));
		put_lex(make_op(ExtraType::IFGOTO_OP));
		pl4 = _polis.size();
		put_lex(make_op(ExtraType::WRONGTYPE));
		put_lex(make_op(ExtraType::GOTO_OP));
		pl1 = _polis.size();
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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

		
		put_lex(make_labl(pl2));
		put_lex(make_op(ExtraType::GOTO_OP));

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;
		_polis[pl4] = make_labl(_polis.size());
		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_R))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(")");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:')'", place);

		auto myStmt = new Stmts<T>(_table, this);
		if (myStmt->derivation(it, end) != EmptyString)
			Add(myStmt);
		else
		{
			delete myStmt;
		}

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		put_lex(make_labl(pl1));
		put_lex(make_op(ExtraType::GOTO_OP));
		_polis[pl3] = make_labl(_polis.size());
		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ENDFOR))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("ENDFOR");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'endfor'", place);

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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
		pl2 = _polis.size();
		auto child = new TerminalSymbol<T>(_table, this);
		child->givenName("while");
		Add(child);
		++it;

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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

		eqbool(place);
		pl3 = _polis.size();
		put_lex(make_op(ExtraType::WRONGTYPE));
		put_lex(make_op(ExtraType::IFGOTO_OP));
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_R))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(")");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:')'", place);

		auto myStmt = new Stmts<T>(_table, this);
		if (myStmt->derivation(it, end) != EmptyString)
			Add(myStmt);
		else
		{
			delete myStmt;
		}

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;
		put_lex(make_labl(pl2));
		put_lex(make_op(ExtraType::GOTO_OP));
		_polis[pl3] = make_labl(_polis.size());
		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ENDWHILE))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("endwhile");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'endwhile'", place);
	
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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
		eqbool(place);
		pl2 = _polis.size();
		put_lex(make_op(ExtraType::WRONGTYPE));
		put_lex(make_op(ExtraType::IFGOTO_OP));
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		if (name == TokenName::DELIM && attribute == static_cast<int>(Delim::RBRACKETS_R))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName(")");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:')'", place);

		auto myStmt = new Stmts<T>(_table, this);
		if (myStmt->derivation(it, end) != EmptyString)
			Add(myStmt);
		else
		{
			delete myStmt;
		}
		pl3 = _polis.size();
		put_lex(make_op(ExtraType::WRONGTYPE));
		put_lex(make_op(ExtraType::GOTO_OP));
		_polis[pl2] = make_labl(_polis.size());
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ELSE))
		{
			++it;
			auto myStmt = new Stmts<T>(_table, this);
			if (myStmt->derivation(it, end) != EmptyString)
			{
				Add(myStmt);
			}
			else
			{
				delete myStmt;
			}
		}

		_polis[pl3] = make_labl(_polis.size());
		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

		if (name == TokenName::WORD && attribute == static_cast<int>(ServWord::ENDIF))
		{
			auto child = new TerminalSymbol<T>(_table, this);
			child->givenName("endif");
			Add(child);
			++it;
		}
		else
			throw new MyException("Expected:'endif'", place);

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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

		r1 = *it;
		token = r1.first;
		place = r1.second;
		r2 = token.getValue();
		name = r2.first;
		attribute = r2.second;

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
