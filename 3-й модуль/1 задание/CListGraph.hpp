// Написать реализацию интерфейса IGraph:
// CListGraph, хранящий граф в виде массива списков смежности

#include "IGraph.hpp"
#include <list>
#include <vector>
#include <cassert>


#ifndef INC_1_CLISTGRAPH_HPP
#define INC_1_CLISTGRAPH_HPP

class ListGraph : public IGraph {
public:
    explicit ListGraph(int n) : adjacency_list(n) {}

    ~ListGraph() = default;

    explicit ListGraph(const IGraph &other);

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override;

    size_t VerticesCount() const override {
        return adjacency_list.size();
    }

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    // массив двусвязных списков
    // индекс в массиве - номер вершины, из которой исходит вершина.
    // в списке перечислены номера вершин, в которые можно попасть из этой вершины.
    std::vector<std::list<int>> adjacency_list;

    bool isValidIndex(int index) const;
};

#endif //INC_1_CLISTGRAPH_HPP
