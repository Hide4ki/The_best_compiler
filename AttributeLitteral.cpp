#include "AttributeLitteral.h"

AttributeLitteral::AttributeLitteral(Type type, int index) :_type{ type }, _index{ index }
{
}

bool AttributeLitteral::operator==(const AttributeLitteral &o) const
{
	return (_type == o._type) && (_index == o._index);
}

pair<Type, int> AttributeLitteral::getAttribute()
{
	return pair<Type, int>(_type, _index);
}
