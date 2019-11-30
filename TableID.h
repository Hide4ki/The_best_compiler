#pragma once
#include "TableSymbol.h"
#include "Place.h"
#include "AttributeID.h"

using namespace std;

class TableSymbol;

class AttributeID;

enum class Type;

class TableID
{
private:
	vector<AttributeID> _body;
	TableID() = default;
	~TableID();
public:
	int look(string)const;
	int putl(string);
	bool setType(int, Type);
	bool declare(int);
	Type getType(int);
	string getName(int);
	friend ostream &operator<<(ostream&, const TableID&);
	friend TableSymbol;
};

