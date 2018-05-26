//
// Created by oleg on 25.05.18.
//

#include <cassert>
#include <boost/range/irange.hpp>
#include "CMatrixGraph.hpp"

CMatrixGraph::CMatrixGraph(const IGraph &other) :
        number_of_vertices(other.VerticesCount()),
        adjacency_matrix(number_of_vertices*number_of_vertices) {
    for (int from : boost::irange(0, number_of_vertices)){
        for (int to : other.GetNextVertices(from)){
            setEdge(from, to, true);
        }
    }
};

// граф ориентированный,
// добавление неориентированного ребра как двух ориентированных.
//     от 'from' к 'to'.
//     от 'to' к 'from'.
void CMatrixGraph::AddEdge(int from, int to){
    assert(isValidIndex(from));
    assert(isValidIndex(to));
    setEdge(from, to, true);
    setEdge(to, from, true);
};

// возвращает номера всех вершин, в которые можно попасть из 'vertex'.
std::vector<int> CMatrixGraph::GetNextVertices(int vertex) const{
    assert(isValidIndex(vertex));
    std::vector<int> to_vertices;
    for (int to : boost::irange(0, number_of_vertices)){
        if (edgeExist(vertex, to)) {
            to_vertices.emplace_back(to);
        }
    }
    return to_vertices;
};

// возвращает номера всех вершин, из которых можно попасть в 'vertex'.
std::vector<int> CMatrixGraph::GetPrevVertices(int vertex) const{
    assert(isValidIndex(vertex));
    std::vector<int> from_vertices;
    for (int from : boost::irange(0, number_of_vertices)){
        if (edgeExist(from, vertex)) {
            from_vertices.emplace_back(from);
        }
    }
    return from_vertices;
};

// проверка, что (направленное) ребро существует между вершинами с указанными номерами.
inline bool CMatrixGraph::edgeExist(int from, int to) const{
    return adjacency_matrix[from * number_of_vertices + to];
};

// установка ребра.
inline void CMatrixGraph::setEdge(int from, int to, bool is_exist){
    adjacency_matrix[from * number_of_vertices + to] = is_exist;
};

// проверка, что такая вершина существует в графе.
inline bool CMatrixGraph::isValidIndex(int index) const{
    return 0 <= index && index < number_of_vertices;
}
