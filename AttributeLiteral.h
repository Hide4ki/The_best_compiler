#pragma once
#include <map>
#include "TableLiterals.h"

using namespace std;

enum class Type;

class AttributeLiteral
{
	Type _type;
	int _index;
	AttributeLiteral() = default;
public:
	bool operator==(const AttributeLiteral&)const;
	pair<Type, int> getAttribute();
	AttributeLiteral(Type type, int index);
	friend class TableLiterals;
};
