//
// Created by oleg on 25.05.18.
//

#include <boost/range/irange.hpp>
#include "CListGraph.hpp"


ListGraph::ListGraph(const IGraph &other)
        : ListGraph(other.VerticesCount()) {
    for (int i : boost::irange(0, (int)adjacency_list.size())){
        for (int vertices : other.GetNextVertices(i)){
            adjacency_list[i].push_back(vertices);
        }
    }
}

void ListGraph::AddEdge(int from, int to) {
    assert(isValidIndex(from));
    assert(isValidIndex(to));

    adjacency_list[from].push_back(to);
    adjacency_list[to].push_back(from);
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(isValidIndex(vertex));

    std::vector<int> next_vertices;
    // копируется из списка в массив.
    // возвращаются вершины, в которые можно попасть.
    std::copy(adjacency_list[vertex].begin(),
              adjacency_list[vertex].end(),
              std::back_inserter(next_vertices));
    return next_vertices;
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    // граф неориентированный (хотя каждое ребро хранится как пара значений)
    return GetNextVertices(vertex);
}

bool ListGraph::isValidIndex(int index) const {
    return 0 <= index && index < adjacency_list.size();
}
