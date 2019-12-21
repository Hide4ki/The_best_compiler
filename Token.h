#pragma once
#include <map>
enum class TokenName {NULLTOKEN, ID, DELIM, WORD, LITERAL, ADDRESS};

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
	void setAttribute(OptionalAttribute);
	void setName(TokenName);
	~Token() = default;
};

const Token NullToken(TokenName::NULLTOKEN,0);