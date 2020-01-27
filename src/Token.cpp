#include "Token.h"
#include <locale>

using namespace sqlc;

char const * 
sqlc::tokenRepr(Token t)
{
	switch (t)
	{
		case Token::comment:
			return "--";
		case Token::moduleDecl:
			return "#module";
		case Token::depDecl:
			return "#depends";
		default:
			throw; 
	}
}

size_t 
sqlc::tokenLen(Token t)
{
	switch (t)
	{
		case Token::comment:
			return 2;
		case Token::moduleDecl:
			return 7;
		case Token::depDecl:
			return 8;
		default:
			throw; 
	}
}


bool 
sqlc::startswith(std::string const & str, Token t)
{
	return str.rfind(tokenRepr(t), 0) == 0;
}

std::string 
sqlc::consume(std::string const & str, Token t)
{
	return str.substr(tokenLen(t), std::string::npos);
}

std::string
sqlc::readIdentifier(std::string const & str)
{
	size_t len = 0;
	for (auto const & rune: str)
	{
		if (std::isspace(rune))
		    break;
		len++;
	}
	return str.substr(0, len);
}

std::string 
sqlc::ltrim(std::string const & str)
{
	size_t start = 0;
	for (auto const & rune: str)
	{
		if (!std::isspace(rune))
		    break;
		start++;
	}
	return str.substr(start, std::string::npos);
}
