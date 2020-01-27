//
// Created by sdv on 1/26/20.

#include <string>
#include <iostream>
#include "UIDRealm.t.h"
#include "Graph.t.h"

int main(int argc, char** argv)
{
    sqlc::UIDRealm<std::string> realm;
    std::string alias ("foo");
    if (!realm.is_associated(alias))
    {
        std::cout <<  realm.associate_uid(alias) << std::endl;
    }

    sqlc::Graph<uid_t> G;
    G.insert(1, std::vector<uid_t>({2}));
    G.insert(2, std::vector<uid_t>({3,4}));
    G.insert(3, std::vector<uid_t>());
    G.insert(4, std::vector<uid_t>({5}));
    G.insert(5, std::vector<uid_t>());
    G.insert(6, std::vector<uid_t>({7}));
    G.insert(7, std::vector<uid_t>({4}));

    sqlc::TopSort<uid_t> sorter(&G);
    auto visitation_order = sorter.top_sort();
    for(auto const& node: visitation_order)
        std::cout << node << std::endl;
}
//

