#include "AttributeLiteral.h"

AttributeLiteral::AttributeLiteral(Type type, int index) :_type{ type }, _index{ index }
{
}

bool AttributeLiteral::operator==(const AttributeLiteral &o) const
{
	return (_type == o._type) && (_index == o._index);
}

pair<Type, int> AttributeLiteral::getAttribute()
{
	return pair<Type, int>(_type, _index);
}
