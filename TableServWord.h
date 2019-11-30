#pragma once
#include "TableSymbol.h"

using namespace std;

class TableSymbol;

class TableServWord
{
private:
	TableServWord();
	~TableServWord() = default;
	vector<string> _words;
public:
	string_view operator[](long) const;
	int look(string) const;
	friend ostream &operator<<(ostream&, const TableServWord&);
	friend class TableSymbol;
};

