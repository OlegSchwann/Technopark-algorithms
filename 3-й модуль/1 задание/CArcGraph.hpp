// Необходимо написать реализацию интерфейса IGraph:
// CArcGraph, хранящий граф в виде одного массива пар {from, to}.
// Очень медленно будет работать.

#include "IGraph.hpp"

#ifndef INC_1_CARCGRAPH_HPP
#define INC_1_CARCGRAPH_HPP

class CArcGraph : public IGraph {
public:
    explicit CArcGraph(int n) : number_of_vertices(n) {}

    ~CArcGraph(){};

    explicit CArcGraph(const IGraph &other);

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override;

    size_t VerticesCount() const override {
        return number_of_vertices;
    }

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    // массив начал и концов рёбер.
    std::vector<std::pair<int, int>> edge_array;
    
    bool isValidIndex(int index) const;

    // количество вершин в графе.
    size_t number_of_vertices;
};

#endif //INC_1_CARCGRAPH_HPP
