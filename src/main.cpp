#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <filesystem>

#include "UIDRealm.t.h"
#include "Graph.t.h"
#include "Module.h"

namespace fs = std::filesystem;
using namespace std;
using namespace sqlc;

int main(int const argc, char const * const * const argv)
{
	cout << "[SQLC]" << endl;

    std::string out_path;
    bool do_output (argc == 3);
    if(do_output) out_path = argv[2];

	if (argc < 2) return 1;
    std::string path = argv[1];
	cout << "Path " << path << endl;

	cout << "[Parse]" << endl;
	std::map<string, Module> modules;
    for (auto const & file : fs::directory_iterator(path))
    {
    	//cout << "File " << file.path() << endl;
    	auto module = parseModule(file.path());
    	if (module)
            modules.insert({(*module).name, *module});
    }

    cout << "[Repository]" << endl;
    UIDRealm<string> uid_realm;
    for (auto const & [name, module]: modules)
    {
        uid_realm.associate_uid(name);
        //uid_t uid = uid_realm.associate_uid(name);
    	//cout << "UID " << uid  << ": " << module.name << endl;
    }

    cout << "[Graph]" << endl;
    Graph<uid_t> G;
    for (auto const & [name, module]: modules)
    {
        vector<uid_t> deps;
        for (auto const & dep: module.deps)
        {
            auto dep_uid = uid_realm.lookup_uid(dep);
            if(!dep_uid)
            {
                cout << "Dependency " << dep << " of " << name << " is not present." << endl;
                return 1;
            }
            deps.push_back(*dep_uid);
        }
        G.insert(*uid_realm.lookup_uid(name), deps);
    }

    TopSort<uid_t> sorter (&G);
    cout << "[Top Sort]" << endl;
    auto sorted = sorter.top_sort();
    vector<Module> load_order;

    cout << "[Load Order]" << endl;
    for(auto it = sorted.rbegin(); it != sorted.rend(); it++)
    {
        uid_t uid = *it;
        Module module = modules.at(*uid_realm.lookup_alias(uid));
        load_order.push_back(module);
        //cout << "UID " << uid << ": " << module.name << endl;
    }

    if(do_output)
    {
        cout << "[Output]" << endl;
        cout << "OutPath " << out_path << endl;
        ofstream fout(out_path);
        if(!fout.good())
        {
            cout << "WriteError" << endl;
            return 1;
        }
        for(Module const& module: load_order)
        {
            ifstream fin(module.path);
            if(!fin.good())
            {
                cout << "ReadError " << module.path << endl;
                return 1;
            }
            //cout << "Write " << module.name << endl;
            for(string line; getline(fin, line); )
                fout << line << endl;
        }
    }
}