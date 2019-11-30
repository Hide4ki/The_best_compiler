#pragma once
#include <map>
enum class TokenName {NULLTOKEN, ID, DELIM, WORD, LITERAL};

using namespace std;
using OptionalAttribute = long;

class Token
{
private:
	TokenName _name;
	OptionalAttribute _attribute;
public:
	Token(TokenName, OptionalAttribute);
	pair<TokenName, OptionalAttribute> getValue();
	~Token() = default;
};

const Token NullToken(TokenName::NULLTOKEN,0);