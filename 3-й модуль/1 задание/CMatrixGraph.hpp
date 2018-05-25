// Необходимо написать несколько реализаций интерфейса  IGraph:
// CMatrixGraph, хранящий граф в виде матрицы смежности,

#include "IGraph.hpp"

#ifndef INC_1_CMATRIXGRAPH_HPP
#define INC_1_CMATRIXGRAPH_HPP

class CMatrixGraph : public IGraph {
public:
    explicit CMatrixGraph(int n) : number_of_vertices(n), adjacency_matrix(n*n) {}

    ~CMatrixGraph() = default;

    explicit CMatrixGraph(const IGraph &other);

    // граф ориентированный,
    // добавление неориентированного ребра как двух ориентированных.
    //     от 'from' к 'to'.
    //     от 'to' к 'from'.
    void AddEdge(int from, int to) override;

    // количество вершин.
    // номер вершины ∈ [0, VerticesCount)
    size_t VerticesCount() const override {
        return number_of_vertices;
    }

    // возвращает номера всех вершин, в которые можно попасть из 'vertex'.
    std::vector<int> GetNextVertices(int vertex) const override;

    // возвращает номера всех вершин, из которых можно попасть в 'vertex'.
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    const int number_of_vertices;

    // матрица смежности.
    //   012
    //  +---to
    // 0| .
    // 1|
    // 2|
    // from
    // adjacency_matrix[from * number_of_vertices + to];
    std::vector<bool> adjacency_matrix;

    // проверка, что (направленное) ребро существует между вершинами с указанными номерами.
    inline bool edgeExist(int from, int to) const;

    // установка ребра.
    inline void setEdge(int from, int to, bool is_exist);

    // проверка, что такая вершина существует в графе.
    inline bool isValidIndex(int index) const;
};

#endif //INC_1_CMATRIXGRAPH_HPP
