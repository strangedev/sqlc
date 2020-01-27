#ifndef SQLC_TOKEN_H
#define SQLC_TOKEN_H

#include <string>

namespace sqlc {

enum Token
{
	comment,
	moduleDecl,
	depDecl
};

static inline Token const Tokens[] = {
	Token::comment,
	Token::moduleDecl,
	Token::depDecl
};

char const * 
tokenRepr(Token t);

size_t 
tokenLen(Token t);

bool 
startswith(std::string const & str, Token t);

std::string 
consume(std::string const & str, Token t);

std::string
readIdentifier(std::string const & str);

std::string 
ltrim(std::string const & str);
	
} // namespace sqlc

#endif // SQLC_TOKEN_H