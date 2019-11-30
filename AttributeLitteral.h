#pragma once
#include <map>
#include "TableLitteral.h"

using namespace std;

enum class Type;

class AttributeLitteral
{
	Type _type;
	int _index;
	AttributeLitteral() = default;
public:
	bool operator==(const AttributeLitteral&)const;
	pair<Type, int> getAttribute();
	AttributeLitteral(Type type, int index);
	friend class TableLitteral;
};
