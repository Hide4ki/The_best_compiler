#include "TableServWord.h"

TableServWord::TableServWord()
{
	_words.reserve(numberServWord);
	_words.resize(numberServWord);
	_words[static_cast<int>(ServWord::BOOL)] = "bool";
	_words[static_cast<int>(ServWord::DO)] = "do";
	_words[static_cast<int>(ServWord::ELSE)] = "else";
	_words[static_cast<int>(ServWord::END)] = "END";
	_words[static_cast<int>(ServWord::ENDFOR)] = "endfor";
	_words[static_cast<int>(ServWord::ENDIF)] = "endif";
	_words[static_cast<int>(ServWord::ENDWHILE)] = "endwhile";
	_words[static_cast<int>(ServWord::FALSE)] = "false";
	_words[static_cast<int>(ServWord::FOR)] = "for";
	_words[static_cast<int>(ServWord::IF)] = "if";
	_words[static_cast<int>(ServWord::INTEGER)] = "integer";
	_words[static_cast<int>(ServWord::PROGRAMM)] = "PROGRAMM";
	_words[static_cast<int>(ServWord::STRING)] = "string";
	_words[static_cast<int>(ServWord::TRUE)] = "true";
	_words[static_cast<int>(ServWord::WHILE)] = "while";
}

string_view TableServWord::operator[](long i) const
{
	return _words[i];
}

int TableServWord::look(string value) const
{
	auto a = find(_words.begin(), _words.end(), value);
	return a != _words.end() ? distance(_words.begin(), a)+1 : 0;
}

ostream & operator<<(ostream &stream, const TableServWord &table)
{
	int i = 0;
	stream << "ServWord Table" << endl;
	stream << " id|val" << endl;
	for (auto a : table._words)
	{
		stream << right << setw(3) << ++i << "|" << "\"" << a << "\"" << endl;
	}
	return stream;
}
