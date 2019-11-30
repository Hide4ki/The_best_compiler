#pragma once
#include "TableID.h"

using namespace std;

enum class Type;

class AttributeID
{
private:
	AttributeID() = default;
	string _name;
	Type _type;
	bool _decl{ false };
	int _indexValue;
	friend class TableID;
public:
	AttributeID(string name);
	bool operator==(const AttributeID&)const;
	string getName()const { return _name; };
};