/*  Задача 1. «Представление графа».(5 баллов)
    Дан базовый интерфейс для представления графа:
    Необходимо написать несколько реализаций интерфейса:
        CListGraph, хранящий граф в виде массива списков смежности,
        CMatrixGraph, хранящий граф в виде матрицы смежности,
        CSetGraph, хранящий граф в виде массива хэш-таблиц,
        CArcGraph, хранящий граф в виде одного массива пар {from, to}.
    Также необходимо реализовать конструктор, принимающий const IGraph*.
    Он должен скопировать переданный граф в создаваемый объект.
    Для каждого класса создавайте отдельные .hpp и .cpp файлы.
    Число вершин графа задается в конструкторе каждой реализации. */

#include <iostream>
#include "CArcGraph.hpp"
#include "CListGraph.hpp"
#include "CMatrixGraph.hpp"
#include "CSetGraph.hpp"

int main() {
    ListGraph list_graph(5);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(1, 3);
    list_graph.AddEdge(2, 4);
    list_graph.AddEdge(3, 4);
    std::cout << list_graph << "\n";

    CArcGraph arc_graph(list_graph);
    std::cout << arc_graph << "\n";

    CMatrixGraph matrix_graph(arc_graph);
    std::cout << matrix_graph << "\n";

    CSetGraph set_graph(matrix_graph);
    std::cout << set_graph << "\n";

    return 0;
}