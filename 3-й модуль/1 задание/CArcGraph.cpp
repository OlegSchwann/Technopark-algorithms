//
// Created by oleg on 25.05.18.
//

#include <cassert>
#include "CArcGraph.hpp"
#include <boost/range/irange.hpp>

CArcGraph::CArcGraph(const IGraph &other) : number_of_vertices(other.VerticesCount()){
    for (int i : boost::irange(0, (int)number_of_vertices)){
        for(int vertice : other.GetNextVertices(i)){
            edge_array.push_back({i, vertice});
        }
    }
};

// Добавление ребра от from к to.
void CArcGraph::AddEdge(int from, int to) {
    assert(isValidIndex(from));
    assert(isValidIndex(to));
    edge_array.emplace_back(std::pair <int, int>(from, to));
    edge_array.emplace_back(std::pair <int, int>(to, from));
};

std::vector<int> CArcGraph::GetNextVertices(int vertex) const{
    assert(isValidIndex(vertex));
    std::vector<int> next_vertices;
    for(auto edge: edge_array){
        if (edge.first == vertex){
            next_vertices.push_back(edge.second);
        }
    }
    return next_vertices;
};

std::vector<int> CArcGraph::GetPrevVertices(int vertex) const{
    assert(isValidIndex(vertex));
    std::vector<int> next_vertices;
    for(auto edge: edge_array){
        if (edge.second == vertex){
            next_vertices.push_back(edge.first);
        }
    }
    return next_vertices;
}

bool CArcGraph::isValidIndex(int index) const {
    return 0 <= index && index < number_of_vertices;
}