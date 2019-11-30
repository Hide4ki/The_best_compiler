#include "TableLiterals.h"

int TableLiterals::putNum(int value)
{
	auto a = _body.size();
	_body.push_back(AttributeLitteral(Type::INTEGER, _nums.size()));
	_nums.push_back(value);
	return a + 1;
}

int TableLiterals::putStr(string value)
{
	auto a = _body.size();
	_body.push_back(AttributeLitteral(Type::STRING, _strs.size()));
	_strs.push_back(value);
	return a + 1;
}

int TableLiterals::putBool(bool value)
{
	auto a = _body.size();
	_body.push_back(AttributeLitteral(Type::BOOL, _bools.size()));
	_bools.push_back(value);
	return a + 1;
}

string TableLiterals::getStr(int i)
{
	auto[type, index] = _body.at(i).getAttribute();
	return _strs.at(index);
}

bool TableLiterals::getBool(int i)
{
	auto[type, index] = _body.at(i).getAttribute();
	return _bools.at(index);
}

int TableLiterals::getNum(int i)
{
	auto[type, index] = _body.at(i).getAttribute();
	return _nums.at(index);
}

Type TableLiterals::getType(int i)
{
	auto[type, index] = _body.at(i).getAttribute();
	return type;
}

ostream & operator<<(ostream &stream, const TableLiterals&table)
{
	stream <<"Litteral table"<< endl;
	stream << " Id|   Type  | val" << endl;
	auto i = 0;
	for (auto a : table._body)
	{
		auto[type, col] = a.getAttribute();
		string nameType;
		switch (type)
		{
		case Type::INTEGER:
			nameType = "integer";
			stream << right << setw(3) << ++i << "|" << setw(9) << right << nameType << setw(3) << "| \"" << table._nums[col] << "\"" << endl;
			break;
		case Type::BOOL:
			nameType = "bool";
			stream << right << setw(3) << ++i << "|" << setw(9) << right << nameType << setw(3) << "| \"" << table._bools[col] << "\"" << endl;
			break;
		case Type::STRING:
			nameType = "string";
			stream << right << setw(3) << ++i << "|"  << setw(9) << right << nameType << setw(3) << "| \"" << table._strs[col] << "\"" << endl;
			break;
		}
	}
	return stream;
}
