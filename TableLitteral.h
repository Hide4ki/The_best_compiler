#pragma once
#include "TableSymbol.h"
#include "AttributeLitteral.h"

using namespace std;

class TableSymbol;
class AttributeLitteral;

class TableLitteral
{
private:
	vector<AttributeLitteral> _body;
	vector<string> _strs;
	vector<bool> _bools;
	vector<int> _nums;
	TableLitteral() = default;
	~TableLitteral() = default;
public:
	int putNum(int);
	int putStr(string);
	int putBool(bool);
	friend ostream &operator<<(ostream&, const TableLitteral&);
	friend class TableSymbol;
};
