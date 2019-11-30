#pragma once
#include "TableSymbol.h"

using namespace std;

class TableSymbol;

class TableDelim
{
private:
	TableDelim();
	~TableDelim() = default;
	vector<string> _picture;
public:
	string_view operator[](long) const;
	friend ostream &operator<<(ostream&, const TableDelim&);
	int look(string)const;
	friend class TableSymbol;
};

