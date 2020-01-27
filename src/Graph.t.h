//
// Created by sdv on 1/25/20.
//

#ifndef SQLC_GRAPH_T_H
#define SQLC_GRAPH_T_H

#include <vector>
#include <map>
#include <stack>
#include "Module.h"

namespace sqlc
{
    enum GraphError
    {
        NodeAlreadyInserted,
        NodeNotInGraph
    };

    template<typename T>
    class Graph
    {
    public:
        Graph();
        ~Graph();
        void insert(T uid, std::vector<T> neighbors);
        std::vector<T> get_neighbors(T uid);
        std::vector<T> get_nodes();
    private:
        std::map<T, std::vector<T>> uid_to_neighbors;
    };

    template<typename T>
    Graph<T>::Graph():
            uid_to_neighbors(std::map<T, std::vector<T>>())
    { }

    template<typename T>
    void Graph<T>::insert(T uid, std::vector<T> neighbors)
    {
        if(uid_to_neighbors.find(uid) != uid_to_neighbors.end())
            throw NodeAlreadyInserted;
        uid_to_neighbors.insert({uid, neighbors});
    }

    template<typename T>
    std::vector<T> Graph<T>::get_neighbors(T uid)
    {
        auto entry = uid_to_neighbors.find(uid);
        if(entry == uid_to_neighbors.end())
            throw NodeNotInGraph;
        return uid_to_neighbors.at(uid);
    }

    template<typename T>
    std::vector<T> Graph<T>::get_nodes()
    {
        std::vector<T> nodes;
        for(auto const& [node, _] : uid_to_neighbors)
            nodes.push_back(node);
        return nodes;
    }

    enum TopSortError
    {
        TopSortIsDead
    };

    template<typename T>
    Graph<T>::~Graph() = default;

    template <typename T>
    class TopSort
    {
    public:
        explicit TopSort(Graph<T>* _graph);
        ~TopSort() = default;
        std::vector<T> top_sort();
        bool is_dead();
    private:
        std::vector<T> top_sort_part(T start);
        Graph<T>* graph;
        std::vector<T> nodes;
        std::vector<T> visitation_order;
        std::set<T> visited;
        bool dead;
    };

    template<typename T>
    TopSort<T>::TopSort(Graph<T> *_graph):
        graph(_graph),
        nodes(_graph->get_nodes()),
        visitation_order(std::vector<T>()),
        visited(std::set<T>()),
        dead(false)
{ }

    template<typename T>
    std::vector<T> TopSort<T>::top_sort()
    {
        if(dead)
            throw TopSortIsDead;

        for(auto const& node: nodes)
        {
            if(visited.find(node) == visited.end())
            {
                auto local_visit_order = top_sort_part(node);
                visitation_order.insert(
                        visitation_order.begin(),
                        local_visit_order.begin(),
                        local_visit_order.end()
                        );
            }
        }

        dead = true;
        return visitation_order;
    }

    template<typename T>
    std::vector<T> TopSort<T>::top_sort_part(T start)
    {
        std::vector<T> local_visit_order;
        std::stack<T> to_visit;
        to_visit.push(start);
        while(!to_visit.empty())
        {
            T cur_node = to_visit.top();
            to_visit.pop();

            if(visited.find(cur_node) != visited.end())
                continue;

            visited.insert(cur_node);
            local_visit_order.push_back(cur_node);

            for(auto const& node: graph->get_neighbors(cur_node))
            {
                if(visited.find(node) != visited.end())
                    continue;

                to_visit.push(node);
            }
        }

        return local_visit_order;
    }

    template<typename T>
    bool TopSort<T>::is_dead()
    {
        return dead;
    }

}

#endif //SQLC_GRAPH_T_H
