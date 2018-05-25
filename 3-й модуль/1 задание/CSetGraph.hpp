// Необходимо написать реализацию интерфейса IGraph:
// CSetGraph, хранящий граф в виде массива хэш-таблиц,

#include "IGraph.hpp"
#include <unordered_set>

#ifndef INC_1_CSETGRAPH_HPP
#define INC_1_CSETGRAPH_HPP

class CSetGraph : public IGraph {
public:
    explicit CSetGraph(int n);

    ~CSetGraph() = default;

    explicit CSetGraph(const IGraph &other);

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override;

    size_t VerticesCount() const override{
        return array.size();
    };

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    // граф хранится как массив хештаблиц.
    // индекс в массиве - номер вершины, из которой исходит ребро.
    // хештаблица хранит номера всех вершин, в которые можно попасть.
    std::vector<std::unordered_set<int>> array;

    bool isValidIndex(int index) const;
};
#endif //INC_1_CSETGRAPH_HPP
