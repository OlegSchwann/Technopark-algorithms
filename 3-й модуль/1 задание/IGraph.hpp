// Базовый интерфейс для представления графа.
// Также в наследниках необходимо реализовать конструктор, принимающий const IGraph*.
// Такой конструктор должен скопировать переданный граф в создаваемый объект.
// Число вершин графа задается в конструкторе каждой реализации.

#include <vector>
#include <cstddef>
#include <ostream>

#ifndef INC_1_IGRAPH_HPP
#define INC_1_IGRAPH_HPP

struct IGraph {
public:
    virtual ~IGraph(){};

    // граф ориентированный,
    // добавление неориентированного ребра
    //     от 'from' к 'to'.
    //     от 'to' к 'from'.
    virtual void AddEdge(int from, int to) = 0;

    // количество вершин.
    // номер вершины ∈ [0, VerticesCount)
    virtual size_t VerticesCount() const = 0;

    // возвращает номера всех вершин, в которые можно попасть из 'vertex'.
    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    // возвращает номера всех вершин, из которых можно попасть в 'vertex'.
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;

    // вывод графа в виде строки [1 ⟶ 2, 2 ⟶ 4].
    friend std::ostream& operator<<(std::ostream& os, const IGraph& graph);
};

#endif //INC_1_IGRAPH_HPP
