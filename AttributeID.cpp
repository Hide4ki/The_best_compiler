#include "AttributeID.h"

AttributeID::AttributeID(string name) :_name{ name } 
{
}

bool AttributeID::operator==(const AttributeID &o) const
{
	return _name == o._name;
}
