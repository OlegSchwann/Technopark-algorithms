// Статический анализатор фатально подавился шаблоном
// std::vector<std::unordered_set<int>> array;
// Плдчёркивает валидный с точки зрения компилятора код.

#include <cassert>
#include <algorithm>
#include <boost/range/irange.hpp>
#include "CSetGraph.hpp"

CSetGraph::CSetGraph(int n) : array(n) {

}

CSetGraph::CSetGraph(const IGraph &other) : array(other.VerticesCount()) {
    for (int from : boost::irange(0, (int) array.size())) {
        std::vector<int> next_vertices = other.GetNextVertices(from);
        for (int vertice : next_vertices) {
            array[from].insert(vertice);
        }
    }
};

// Добавление ребра от from к to.
void CSetGraph::AddEdge(int from, int to) {
    assert(isValidIndex(from));
    assert(isValidIndex(to));

    array[from].emplace(to);
    array[to].emplace(from);
};


std::vector<int> CSetGraph::GetNextVertices(int vertex) const {
    std::vector<int> next_vertices;
    for (int i : array[vertex]) {
        next_vertices.emplace_back(i);
    }
    return next_vertices;
};

std::vector<int> CSetGraph::GetPrevVertices(int vertex) const {
    return GetNextVertices(vertex);
};

bool CSetGraph::isValidIndex(int index) const {
    return 0 <= index && index < array.size();
}