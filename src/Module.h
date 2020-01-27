#ifndef SQLC_MODULE_H
#define SQLC_MODULE_H

#include <string>
#include <set>
#include <optional>
#include "Token.h"
#include "UIDRealm.t.h"

namespace sqlc {

class Module
{
public:
	std::string const path;
	std::string const name;
	std::set<std::string> const deps;

	Module(
		std::string _path,
		std::string _uid,
		std::set<std::string> _deps
	);
	~Module();
}; // class Module 

enum ParseError
{
	IOError,
	DuplicateModuleDeclaration,
	DuplicateDependencyDeclaration
};

std::optional<Module> parseModule(std::string const & path);

} // namespace sqlc

#endif // SQLC_MODULE_H