#include "TableID.h"

TableID::~TableID()
{
}

int TableID::look(string value)const
{
	auto a = find(_body.begin(), _body.end(), value);
	return a != _body.end() ? distance(_body.begin(), a)+1 : 0;
}

int TableID::putl(string name)
{
	auto a = _body.size();
	int i;
	if (!(i = look(name)))
	{
		_body.push_back(AttributeID(name));
		return a + 1;
	}
	return i;
}

bool TableID::setType(int i, Type type)
{
	if (!_body.at(i)._decl)
	{
		_body.at(i)._decl = true;
		_body.at(i)._type = type;
		return true;
	}
	return false;
}

string TableID::getName(int i)
{
	return _body.at(i)._name;
}

ostream & operator<<(ostream &stream, const TableID &table)
{
	stream << "Table ID" << endl;
	stream << " id|val" << endl;
	auto i = 0;
	for (auto a : table._body)
		stream << setw(3) << right << ++i << "|" << "\"" << a.getName() << "\"" << endl;
	return stream;
}
