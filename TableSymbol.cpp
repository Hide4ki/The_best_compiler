#include "TableSymbol.h"

TableSymbol::TableSymbol()
{
	_tableId = new TableID;
	_tableDelim = new TableDelim;
	_tableServWord = new TableServWord;
	_tableLitteral = new TableLiterals;
}

TableSymbol::~TableSymbol()
{
	delete _tableId;
	delete _tableDelim;
	delete _tableServWord;
	delete _tableLitteral;
}

TableID &TableSymbol::getTableID()
{
	return *_tableId;
}

const TableDelim &TableSymbol::getTableDelim()const
{
	return *_tableDelim;
}

const TableServWord &TableSymbol::getTableServWord()const
{
	return *_tableServWord;
}

TableLiterals &TableSymbol::getTableLiterals()
{
	return *_tableLitteral;
}
