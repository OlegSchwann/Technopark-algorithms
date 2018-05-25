//
// Created by oleg on 25.05.18.
//

#include "CListGraph.hpp"

ListGraph::ListGraph(const IGraph &other)
        : ListGraph(other.VerticesCount()) {
    for (int i = 0; i < adjacency_list.size(); i++) {
        std::vector<int> nextVerticies = other.GetNextVertices(i);
        std::copy(nextVerticies.begin(),
                  nextVerticies.end(),
                  std::back_inserter(adjacency_list[i]));
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

//это зачем?
//template<class TFunc>
//void bfs(const IGraph &graph, int start, TFunc operation) {
//    std::queue<int> q;
//    std::vector<bool> isQueued(graph.VerticesCount(), false);
//
//    q.push(start);
//    isQueued[start] = true;
//    while (!q.empty()) {
//        int v = q.front();
//        q.pop();
//        operation(q);
//        for (int next : graph.GetNextVertices(v)) {
//            if (!isQueued[next]) {
//                q.push(next);
//                isQueued[next] = true;
//            }
//        }
//    }
//}