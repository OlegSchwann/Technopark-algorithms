/*  https://contest.yandex.ru/contest/8213/problems/
    Задача 2. Количество различных путей (3 балла).
    0.2 сек, 10Mb
    Дан невзвешенный неориентированный граф.
    В графе может быть несколько кратчайших путей между какими-то вершинами.
    Найдите количество различных кратчайших путей между заданными вершинами.
    Требуемая сложность O(V+E) - пропорционально количеству вершин и рёбер.
    Формат ввода.
    v: кол-во вершин (макс. 50000),
    n: кол-во ребер (макс. 200000),
    n пар реберных вершин,
    пара вершин u, w для запроса.
    Формат вывода.
    Количество кратчайших путей от u к w.
  1
 /|\
0 | 3
 \|/
  2
in
4
5
0 1
0 2
1 2
1 3
2 3

0 3

out
2

 */
/* https://stackoverflow.com/questions/10226251/#10227808
  1   4   7   10
 / \ / \ / \ /  \
0   3   6   9    12
 \ / \ / \ / \  /
  2   5   8   11

in
13
16
0 1
0 2
1 3
2 3
3 4
3 5
4 6
5 6
6 7
6 8
7 9
8 9
9 10
9 11
10 12
11 12

0 12

out
16
 */

#include <vector>
#include <list>
#include <cassert>
#include <queue>
#include <cstdint>
#include <iostream>


/*  одна вершина графа:
        номер,
        текущая минимальная длинна пути,
        количество входящих путей с текущей минимальной длинной,
        список вершин, в которые можно попасть из этой.
 */
class Node {
public:
    // id = номер узла как индекс в массиве.
    int minimum_path;
    int number_of_incoming;
    std::list<int> id_connected_vertices;

    Node() : number_of_incoming(0), minimum_path(INT32_MAX), id_connected_vertices() {};

    ~Node() = default;
};

// Дан невзвешенный неориентированный граф.
class ListGraph {
public:
    explicit ListGraph(int n) : nodes(n), from(-1) {}

    ~ListGraph() = default;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to);

    // возвращает количество кратчайших путей между двумя вершинами.
    int NumberOfShortestPaths(int from, int to);

private:
    // массив двусвязных списков
    // индекс в массиве - номер вершины, из которой исходит вершина.
    // номера вершин, в которые можно попасть из этой вершины перечислены в Node::id_connected_vertices;
    std::vector<Node> nodes;
    // поле, хранит -1, если ещё не построены пути,
    // или номер вершины, кратчайшие пути из которой в данный момент записаны в вершинах.
    int from;

    bool isValidIndex(int index) const {
        return 0 <= index && index < nodes.size();
    };

    // непосредственно алгоритм, который размечает все вершины по длинне пути и количеству кратчайших путей.
    // сохраняет эти данные непосредственно в this->nodes; и устанавливает this->from = from;
    void setNumbersOfShortestPaths(int from);
};

// Добавление ребра от from к to.
void ListGraph::AddEdge(int from, int to) {
    assert(isValidIndex(from));
    assert(isValidIndex(to));
    // тут надо бы проверить на уникальность ребра, но тесты заведомо корректны.
    nodes[from].id_connected_vertices.push_back(to);
    nodes[to].id_connected_vertices.push_back(from);
}

// возвращает количество кратчайших путей между двумя вершинами.
int ListGraph::NumberOfShortestPaths(int from, int to) {
    if (this->from == from) { // если уже посчитано
        return nodes[to].number_of_incoming;
    } else if (this->from == -1) {
        setNumbersOfShortestPaths(from);
        return nodes[to].number_of_incoming;
    } else {
        // тут надо подготовить граф снова для работы алгоритма
        // выставить рассояние в max_int и количество входящих в 0.
        throw std::exception();
    }
};

/*
    Используем граф на списках.
    У каждой вершины дополнительно хранится количество путей, которыми можно добраться до этой вершины и расстояние.

    алгоритм обхода:
    берём начальную и конечную вершину.
    заводим очередь обхода, кладём туда начальную вершину.

    while(очередь не пуста){
        вытаскиваем из очереди вершину.(и не возвращаем потом.)
        для каждой дочерней вершины:
            путь от текущей = текущая.минимальное расстояние + 1
            если количество входящих == 0
                ставим количество входящих = 1;
                расстояние = путь от текущей;
                добавляем id в очередь
            иначе
                если дочерняя вершина.путь < путь от текущей
                    пропускаем
                иначе дочерняя вершина.путь = путь от текущей
                    увеличиваем количество путей сюда на количество путей в текущую вершину
                иначе // дочерняя вершина.путь > путь от текущей
                    устанавливаем путь от текущей как путь в дочерней вершине
                    количество путей до неё
    }
    в конце вытащить по индексу конечной вершины количество кратчайших путей
 */

void ListGraph::setNumbersOfShortestPaths(int from) {
    std::queue<int> vertices_ids; // очередь, содержащая рассматриваемый слой вершин.
    vertices_ids.push(from);
    nodes[from].number_of_incoming = 1;
    nodes[from].minimum_path = 0;
    while (!vertices_ids.empty()) {
        int current_vertice = vertices_ids.front(); // текущая рассматриваемая вершина.
        vertices_ids.pop();
        // тут единица - вес любого ребра невзвешенного графа.
        int new_path = nodes[current_vertice].minimum_path + 1;
        for (int to : nodes[current_vertice].id_connected_vertices) {
            // если мы ещё не были в этой вершине
            if (nodes[to].number_of_incoming == 0) {
                vertices_ids.push(to);
                // Добавляем путей ровно столько, сколькими можно прийти в текущую вершину.
                nodes[to].number_of_incoming += nodes[current_vertice].number_of_incoming;
                nodes[to].minimum_path = new_path;
            } else { // если мы были в этой вершине
                // если путь, которым мы пришли - короче, чем в данный момент указанный
                if (new_path < nodes[to].minimum_path) {
                    nodes[to].minimum_path = new_path;
                    nodes[to].number_of_incoming = nodes[current_vertice].number_of_incoming;
                    // если путь такой же длинны
                } else if (new_path == nodes[to].minimum_path) {
                    // Добавляем путей ровно столько, сколькими можно прийти в текущую вершину.
                    nodes[to].number_of_incoming += nodes[current_vertice].number_of_incoming;
                } // а если пришли более длинным путём - ничего не делаем
            }
        }
    }
}

int main() {
    // Формат ввода.
    // v: кол-во вершин (макс. 50000),
    int vertex = 0;
    std::cin >> vertex;
    ListGraph list_graph(vertex);
    // n: кол-во ребер (макс. 200000),
    int edges = 0;
    std::cin >> edges;
    // n пар реберных вершин,
    for(int i = 0; i < edges; ++i){
        int from(0), to(0);
        std::cin >> from >> to;
        list_graph.AddEdge(from, to);
    }
    // пара вершин u, w для запроса.
    int from(0), to(0);
    std::cin >> from >> to;
    // Формат вывода.
    // Количество кратчайших путей от u к w.
    std::cout << list_graph.NumberOfShortestPaths(from, to) << std::endl;
    return 0;
}

