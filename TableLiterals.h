#pragma once
#include "TableSymbol.h"
#include "AttributeLitteral.h"

using namespace std;

class TableSymbol;
class AttributeLitteral;

class TableLiterals
{
private:
	vector<AttributeLitteral> _body;
	vector<string> _strs;
	vector<bool> _bools;
	vector<int> _nums;
	TableLiterals() = default;
	~TableLiterals() = default;
public:
	int putNum(int);
	int putStr(string);
	int putBool(bool);
	string getStr(int);
	bool getBool(int);
	int getNum(int);
	Type getType(int);
	friend ostream &operator<<(ostream&, const TableLiterals&);
	friend class TableSymbol;
};
