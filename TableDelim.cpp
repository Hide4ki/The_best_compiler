#include "TableDelim.h"

TableDelim::TableDelim()
{
	_picture.reserve(numberDelim);
	_picture.resize(numberDelim);
	_picture[static_cast<int>(Delim::ADD_OP)] = "+";
	_picture[static_cast<int>(Delim::AND_OP)] = "&&";
	_picture[static_cast<int>(Delim::ASS_OP)] = "=";
	_picture[static_cast<int>(Delim::COMMA)] = ",";
	_picture[static_cast<int>(Delim::DIV_OP)] = "/";
	_picture[static_cast<int>(Delim::DQUOTES)] = "\"";
	_picture[static_cast<int>(Delim::EQ_OP)] = "==";
	_picture[static_cast<int>(Delim::INC_OP)] = "++";
	_picture[static_cast<int>(Delim::LARGE_OP)] = ">";
	_picture[static_cast<int>(Delim::NOT_OP)] = "!";
	_picture[static_cast<int>(Delim::LESS_OP)] = "<";
	_picture[static_cast<int>(Delim::MUL_OP)] = "*";
	_picture[static_cast<int>(Delim::NEG_OP)] = "-";
	_picture[static_cast<int>(Delim::NOTEQ_OP)] = "!=";
	_picture[static_cast<int>(Delim::OR_OP)] = "||";
	_picture[static_cast<int>(Delim::RBRACKETS_L)] = "(";
	_picture[static_cast<int>(Delim::RBRACKETS_R)] = ")";
	_picture[static_cast<int>(Delim::SEMICOLON)] = ";";
	_picture[static_cast<int>(Delim::SQUOTES)] = "'";
	_picture[static_cast<int>(Delim::BRACKETS_L)] = "{";
	_picture[static_cast<int>(Delim::BRACKETS_R)] = "}";
}

string_view TableDelim::operator[](long i) const
{
	return _picture[i - 1];
}

int TableDelim::look(string value)const
{
	auto a = find(_picture.begin(), _picture.end(), value);
	return a != _picture.end() ? distance(_picture.begin(), a)+1 : 0;
}

ostream & operator<<(ostream &stream, const TableDelim &table)
{
	int i = 0;
	stream << "Delim Table" << endl;
	stream << " id|val" << endl;
	for (auto a : table._picture)
	{
		if (i !=0)
		stream << right << setw(3) << i << "|" << "\"" <<a << "\"" << endl;
		i++;
	}
	return stream;
}
