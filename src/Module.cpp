#include "Module.h"
#include <iostream>
#include <fstream>
#include <utility>

using namespace std;
using namespace sqlc;

Module::Module(
		string _path,
		string _name,
		set<string> _deps
	):
		path(std::move(_path)),
		name(std::move(_name)),
		deps(std::move(_deps))
{ }

Module::~Module()
= default;

optional<Module> sqlc::parseModule(string const & path)
{
	optional<Module> module;
	optional<string> name;
	set<string> deps;

	setlocale(LC_ALL, "");
	ifstream source (path, ifstream::in);
	if (!source.good()) 
		throw ParseError::IOError;

	for (string line; getline(source, line); )
	{
		if (!startswith(line, Token::comment))
			continue;

		line = ltrim(consume(line, Token::comment));

		if (startswith(line, Token::moduleDecl))
		{
			if (name)
				throw ParseError::DuplicateModuleDeclaration;
			line = ltrim(consume(line, Token::moduleDecl));
			if(line.length() > 0)
			    name.emplace(readIdentifier(line));
		}
		else if (startswith(line, Token::depDecl))
		{
			line = ltrim(consume(line, Token::depDecl));
			string dep = readIdentifier(line);
			if (deps.count(dep) != 0)
				throw ParseError::DuplicateDependencyDeclaration;
			deps.insert(dep);
		}
	}

	if (name)
		module.emplace(Module(path, *name, deps));

	return module;
}