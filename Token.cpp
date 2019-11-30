#include "Token.h"

Token::Token(TokenName name, OptionalAttribute attribute) :_name{ name }, _attribute{ attribute }
{
}

pair<TokenName, OptionalAttribute> Token::getValue()
{
	return pair<TokenName, OptionalAttribute>(_name, _attribute-1);
}