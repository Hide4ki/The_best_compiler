#pragma once
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>
#include <iomanip>
#include "TableID.h"
#include "TableLiterals.h"
#include "TableDelim.h"
#include "TableServWord.h"
#include "Interpretator.h"

enum class Delim { COMMA = 1, DQUOTES, SQUOTES, SEMICOLON, OR_OP, AND_OP, EQ_OP, NOTEQ_OP, ASS_OP, LARGE_OP, LESS_OP, INC_OP, ADD_OP, DIV_OP, MUL_OP, NOT_OP, NEG_OP, RBRACKETS_L, RBRACKETS_R, BRACKETS_L, BRACKETS_R};

const int numberDelim = static_cast<int>(Delim::BRACKETS_R) + 1;

enum class ServWord { PROGRAMM = 1, TRUE, FALSE, END, INTEGER, BOOL, STRING, FOR, WHILE, IF, ELSE, DO, ENDWHILE, ENDIF, ENDFOR };

const int numberServWord = static_cast<int>(ServWord::ENDFOR) + 1;

enum class Type { INTEGER, STRING, BOOL };

template <class T>
class Interpretator;

class TableID;
class TableLiterals;
class TableServWord;
class TableDelim;

class TableSymbol
{
private:
	TableID *_tableId;
	TableDelim *_tableDelim;
	TableServWord *_tableServWord;
	TableLiterals *_tableLitteral;
	TableSymbol();
	~TableSymbol();
public:
	TableID &getTableID();
	const TableDelim &getTableDelim() const;
	const TableServWord &getTableServWord() const;
	TableLiterals &getTableLiterals();
	template <class T>
	friend class Interpretator;
};

