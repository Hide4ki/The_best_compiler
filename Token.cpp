#include "Token.h"

Token::Token(TokenName name, OptionalAttribute attribute) :_name{ name }, _attribute{ attribute }
{
}

pair<TokenName, OptionalAttribute> Token::getValue()
{
	return pair<TokenName, OptionalAttribute>(_name, _attribute-1);
}

void Token::setAttribute(OptionalAttribute a)
{
	_attribute = a;
}

void Token::setName(TokenName a)
{
	_name = a;
}
