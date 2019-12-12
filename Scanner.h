#pragma once
#include <map>
#include <string>
#include "TableSymbol.h"
#include "Token.h"
#include "Place.h"
#include "Buffer.h"
#include "Interpretator.h"
#include "LexIterator.h"

enum class State { START, OR, CHAR, AND, ADD, ID, NEQ, NUM, STR, STCOM, COM, FIN, ASS, ER, DLM};

class TableSymbol;

template <class T>
class Interpretator;

template <class T>
class LexIterator;

template <class T>
class Scanner
{
private:
	Token *_token;
	Token _returnedToken{ NullToken };
	TableSymbol *_table;
	State _st;
	Buffer<T> *_inputBuf;
	string _buffer;
	Place _stLex;
	bool _rToken{ false };
	char _c; 
	long _d; 
	int _j;  
	void id_or_word();
	void is_dlm();
	Scanner();
	Scanner(T, T);
	~Scanner();
	void SetTable(TableSymbol*);
public:
	Scanner<T> &nextToken();
	bool operator!=(const Scanner<T>&)const;
	pair<Token, Place> getToken();
	void returnToken(Token);
	friend Interpretator<T>;
	friend LexIterator<T>;
};

template<class T>
void Scanner<T>::id_or_word()
{
	auto a = _table->getTableServWord().look(_buffer);
	_token = a ? new Token(TokenName::WORD, a) : new Token(TokenName::ID, _table->getTableID().putl(_buffer));
}

template<class T>
void Scanner<T>::is_dlm()
{
	auto a = _table->getTableDelim().look(_buffer);
	if (a)
		_token = new Token(TokenName::DELIM, a);
	else
	{
		_st = State::ER;
		throw new MyException("Undeclarete symbol!", _inputBuf->nowPlace());
	}
}

template<class T>
Scanner<T>::Scanner()
{
	_st = State::FIN;
}

template<class T>
Scanner<T>::Scanner(T it, T end)
{
	_inputBuf = new Buffer<T>(it, end);
}

template<class T>
Scanner<T>::~Scanner()
{
	delete _inputBuf;
}

template<class T>
void Scanner<T>::SetTable(TableSymbol *table)
{
	_table = table;
}

template<class T>
Scanner<T>& Scanner<T>::nextToken()
{
	_st = State::START;
	auto gc = [&] {_c = _inputBuf->gc(); };
	auto add = [&](char c) {_buffer += c; };
	auto clear = [&] {_buffer.clear(); };
	auto memPlace = [&] {_stLex = _inputBuf->nowPlace(); };
	auto changeState = [&](State st) {_st = st; };
	auto assert = [&](char c)->bool {return _c == c; };
	auto backSymb = [&] {_inputBuf->returnSymb(); };
	gc();
	delete _token;
	_token = nullptr;
	while(1)
	{
		switch(_st)
		{
		case State::START:
			memPlace();
			clear();
			if (assert(' '))
				gc();
			else if (isalpha(_c))
			{
				add(_c);
				gc();
				changeState(State::ID);
			}
			else if (isdigit(_c))
			{
				_d = _c - '0';
				gc();
				changeState(State::NUM);
			}
			else if (assert('!'))
			{
				gc();
				changeState(State::NEQ);
			}
			else if (assert('='))
			{
				gc();
				changeState(State::ASS);
			}
			else if (assert('&'))
			{
				gc();
				changeState(State::AND);
			}
			else if (assert('|'))
			{
				gc();
				changeState(State::OR);
			}
			else if (assert('\"'))
			{
				gc();
				changeState(State::STR);
			}
			else if (assert('\''))
			{
				gc();
				changeState(State::CHAR);
			}
			else if (assert('+'))
			{
				gc();
				changeState(State::ADD);
			}
			else if (assert('/'))
			{
				gc();
				changeState(State::STCOM);
			}
			else if (assert(EOF))
			{
				changeState(State::FIN);
				return*this;
			}
			else
			{
				changeState(State::DLM);
			}
			break;
		case State::ID:
			if (isalpha(_c) || isdigit(_c)) 
			{
				add(_c);
				gc();
			}
			else
			{
				backSymb();
				id_or_word();
				return *this;
			}
			break;
		case State::NUM:
			if(isdigit(_c))
			{
				_d = _d * 10 + (_c - '0');
				gc();
			}
			else
			{
				backSymb();
				_token = new Token(TokenName::LITERAL, _table->getTableLiterals().putNum(_d));
				return *this;
			}
			break;
		case State::CHAR:
			add(_c);
			gc();
			if (!assert('\''))
			{
				if (assert(EOF) || assert('\n'))
				{
					throw new MyException("Unexpected end of line or end programm", _inputBuf->nowPlace());
				}
				throw new MyException("Expected \"\'\"", _inputBuf->nowPlace());
			}
			else
			{
				_token = new Token(TokenName::LITERAL, _table->getTableLiterals().putStr(_buffer));
				return *this;
			}
			break;
		case State::STR:
			if (!assert('\"'))
			{
				if (assert(EOF) || assert('\n'))
					throw new MyException("Unexpected end of line or end programm", _inputBuf->nowPlace());
				add(_c);
				gc();
			}
			else
			{
				_token = new Token(TokenName::LITERAL, _table->getTableLiterals().putStr(_buffer));
				return *this;
			}
			break;
		case State::ASS:
			if (assert('='))
			{
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::EQ_OP) + 1);
				return *this;
			}
			else
			{
				backSymb();
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::ASS_OP) + 1);
				return *this;
			}
			break;

		case State::OR:
			if (assert('|'))
			{
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::OR_OP) + 1);
				return *this;
			}
			else
			{
				backSymb();
				throw new MyException("Expected '|'", _inputBuf->nowPlace());
			}
			break;
		case State::AND:
			if (assert('&')) 
			{
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::AND_OP) + 1);
				return *this;
			}
			else
			{
				backSymb();
				throw new MyException("Expected '&'", _inputBuf->nowPlace());
			}
			break;
		case State::NEQ:
			if (assert('='))
			{
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::NOTEQ_OP) + 1);
				return *this;
			}
			else
			{
				backSymb();
				throw new MyException("Expected '='", _inputBuf->nowPlace());
			}
		case State::ADD:
			if (assert('+')) 
			{
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::INC_OP) + 1);
				return *this;
			}
			else
			{
				backSymb();
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::ADD_OP) + 1);
				return *this;
			}
		case State::STCOM:
			if (assert('/'))
			{
				gc();
				_inputBuf->skipLine();
				gc();
				changeState(State::START);
			}
			else if(assert('*')) 
			{
				gc();
				changeState(State::COM);
			}
			else
			{
				backSymb();
				_token = new Token(TokenName::DELIM, static_cast<int>(Delim::DIV_OP) + 1);
				return *this;
			}
			break;
		case State::COM:
			if (assert('*')) 
			{
				gc();
				if (assert('/'))
				{
					gc();
					changeState(State::START);
				}
			}
			else if (assert(EOF)) 
			{
				changeState(State::ER);
				throw new MyException("End of file encountered, but was expected end of multiline comment", _inputBuf->nowPlace());
			}
			else
			{
				gc();
			}
			break;
		default:
			backSymb();
			add(_c);
			if (!assert('\n'))
			{
				is_dlm();
				gc();
				return *this;
			}
			_inputBuf->skipLine();
			gc(); 
			changeState(State::START);
		}
	}
}

template<class T>
bool Scanner<T>::operator!=(const Scanner<T>&)const
{
	return _st != State::FIN;
}

template<class T>
pair<Token, Place> Scanner<T>::getToken()
{
	if (_rToken)
	{
		_rToken = false;
		return pair<Token, Place>(_returnedToken, _stLex);
	}
	if (_st == State::FIN)
		throw new MyException("Unexpected transition to FIN state", _inputBuf->nowPlace());
	auto a = *_token;
	return pair<Token, Place>(a, _stLex);
}

template<class T>
void Scanner<T>::returnToken(Token returnedToken)
{
	_returnedToken = returnedToken;
	_rToken = true;
}
