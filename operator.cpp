#include "operator.h"

ostream &operator <<(ostream &stream, pair<Token, Place> &myType)
{
	auto[token, place] = myType;
	auto[type, record] = token.getValue();
	auto[line, pos] = place.getPlace();
	string nameToken;
	switch (type)
	{
	case TokenName::DELIM:
		nameToken = "Delim";
		break;
	case TokenName::ID:
		nameToken = "identifier";
		break;
	case TokenName::LITERAL:
		nameToken = "litteral";
		break;
	case TokenName::WORD:
		nameToken = "serv word";
		break;
	}
	stream << "<" << nameToken << ", " << record << "> " << line << "|" << pos;
	return stream;
}