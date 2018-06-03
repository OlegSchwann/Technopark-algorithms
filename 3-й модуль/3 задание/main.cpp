// https://contest.yandex.ru/contest/8213/problems/3/
// https://habr.com/post/119158/
// https://neerc.ifmo.ru/wiki/index.php?title=Эвристики_для_поиска_кратчайших_путей
// https://neerc.ifmo.ru/wiki/index.php?title=Алгоритм_Дейкстры
/*
Города
1 секунда, 20Mb
Требуется отыскать самый выгодный маршрут между городами.
Из города может выходить дорога, которая возвращается в этот же город.(Граф с петлями).
Все указанные дороги двусторонние.(Граф неориентированный.)
Между любыми двумя городами может быть больше одной дороги.(Кратные рёбра.)
Требуемое время работы O((N + M)log N), где
    N – количество городов, N ≤ 10000,
    M – известных дорог между ними, M ≤ 250000.
Длина каждой дороги ≤ 10000.

Формат ввода
Первая строка содержит число N – количество городов.
Вторая строка содержит число M - количество дорог.
Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
Последняя строка содержит маршрут (откуда и куда нужно доехать).

Формат вывода
Вывести длину самого выгодного маршрута.

in
6
9
0 3 1
0 4 2
1 2 7
1 3 2
1 4 3
1 5 3
2 5 3
3 4 4
3 5 6
0 2
out
9

in
5
6
0 1 3
0 2 2
0 3 1
1 4 3
2 4 2
3 4 1
0 4
out
2
*/

#include <iostream>
#include <list>
#include <cstdint>
#include <vector>
#include <cassert>
#include <queue>

// тип - id вершины, для читаемости остальных структур.
typedef int VertexId;

// ребро графа
// вершина назначения
// вес ребра
class GraphEdge {
public:
    VertexId to;
    int weight_edge;

    GraphEdge() = delete;

    GraphEdge(VertexId to_vertex, int weight_edge) : to(to_vertex), weight_edge(weight_edge) {};

    ~GraphEdge() = default;
};

// одна вершина графа:
// номер узла - индекс в массиве вершин,
// текущая минимальная длинна пути,
// список рёбер, выходящий из вершины.
// отметка о том, что вершина посещена и её удалённость пересмотру не подлежит.
// В алгоритме Дейкстры кладём в очередь с приоритетом GraphNodePtr всякий раз,
// когда удаётся прорелаксировать ребро.
class GraphNode {
public:
    int minimum_path;
    bool considered;
    std::list<GraphEdge> edges;

    GraphNode() : minimum_path(INT32_MAX), edges(), considered(false) {};

    ~GraphNode() = default;
};

// класс который будем хранить в очереди с приоритетом:
//     указатель на вершину графа,
//     сохранённое расстояние.
//         Так как не существует способа обновить значение в произвольном месте кучи,
//         при обновлении расстояния мы оставляем фиктивный элемент.
//         будет работать быстро, если граф имеет малую связность,
//         и медленно при большой связности и частых релаксациях.
//     operator> будет вызываться std::greater();
//         позволяет упорядочивать GraphNode по saved_priority.
//     isValid()
//         если false в мометн рассмотрения, то просто берём следующий элемент.
class GraphNodePtr {
public:
    int node_id; // for debug
    GraphNode *node;
    int saved_priority;

    // пустой конструктор
    GraphNodePtr() = delete;

    // Деструктор - не надо трогать сам узел графа, он будет собран в деструкторе графа.
    ~GraphNodePtr() = default;

    GraphNodePtr &operator=(const GraphNodePtr &) = default;

//    GraphNodePtr(GraphNode &node) : node(&node), saved_priority(node.minimum_path) {}
    GraphNodePtr(GraphNode &node, int id) : node(&node), saved_priority(node.minimum_path), node_id(id) {}


    bool isValid() {
        return saved_priority == node->minimum_path;
    }

    friend bool operator>(const GraphNodePtr &left, const GraphNodePtr &right) {
        bool result = left.saved_priority > right.saved_priority;
        return result;
    };
};

// Дан невзвешенный неориентированный граф.
class ListGraph {
public:
    // принимает в конструкторе количестро узлов.
    explicit ListGraph(size_t n) : nodes(n), from(-1) {}

    ~ListGraph() = default;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to, int weight);

    // возвращает длину кратчайшего пути между двумя вершинами.
    int LenthOfShortestPaths(int from, int to);

//    void print();

private:
    // массив узлов графа.
    // индекс в массиве - номер вершины, из которой исходит вершина.
    // номера вершин, в которые можно попасть из этой вершины,
    // перечислены в GraphNode::GraphEdge::to;
    std::vector<GraphNode> nodes;
    // поле, хранит -1, если ещё не размечены пути,
    // или номер вершины, кратчайшие пути из которой в данный момент записаны в вершинах.
    int from;

    bool isValidIndex(int index) const {
        return 0 <= index && index < nodes.size();
    };

    // непосредственно алгоритм, который размечает все вершины по длинне пути и количеству кратчайших путей.
    // сохраняет эти данные непосредственно в this->nodes; и устанавливает this->from = from;
    void dextreeAlgorithm(int from);
};

void ListGraph::AddEdge(int from, int to, int weight) {
    assert(isValidIndex(from));
    assert(isValidIndex(to));
    nodes[from].edges.emplace_back(GraphEdge(to, weight));
    nodes[to].edges.emplace_back(GraphEdge(from, weight));
}

int ListGraph::LenthOfShortestPaths(int from, int to) {
    if (this->from != -1) {
        // пройтись и очистить граф
        // int minimum_path = ∞;
        // considered = false;
        assert(false);
        "Вызвали 2-й раз.";
    }
    this->from = from;
    dextreeAlgorithm(from);
    return nodes[to].minimum_path;
};

//void ListGraph::print() {
//    for (int i = 0; i < nodes.size(); ++i) {
//        for (GraphEdge edge : nodes[i].edges) {
//            std::cout << i << " --" << edge.weight_edge << "-> " << edge.to << "\n";
//        }
//    }
//};

/* Алгоритм Дейкстры для разреженных графов. Делает то же самое, что и алгоритм Дейкстры, но за количество операций порядка m * log(n). Следует заметить, что m может быть порядка n^2, то есть эта вариация алгоритма Дейкстры не всегда быстрее классической, а только при маленьких m. Что нам нужно в алгоритме Дейкстры? Нам нужно уметь находить по значению d минимальную вершину и уметь обновлять значение d в какой-то вершине. В классической реализации мы пользуемся простым массивом, находить минимальную по d вершину мы можем за порядка n операций, а обновлять — за 1 операцию. Воспользуемся двоичной кучей. Куча поддерживает операции: добавить в кучу элемент (за порядка log(n) операций), найти минимальный элемент (за 1 операцию), удалить минимальный элемент (за порядка log(n) операций). Создадим массив d[0...n — 1] (его значение то же самое, что и раньше) и кучу q. В куче будем хранить пары из номера вершины v и d[v] (сравниваться пары должны по d[v]). Также в куче могут быть фиктивные элементы. Так происходит, потому что значение d[v] обновляется, но мы не можем изменить его в куче. Поэтому в куче могут быть несколько элементов с одинаковым номером вершины, но с разным значением d (но всего вершин в куче будет не более m, я гарантирую это). Когда мы берём минимальное значение в куче, надо проверить, является ли этот элемент фиктивным. Для этого достаточно сравнить значение d в куче и реальное его значение. А ещё для записи графа вместо двоичного массива используем массив списков. m раз добавляем элемент в кучу, получаем порядка m * log(n) операций. */

void ListGraph::dextreeAlgorithm(int from) {
    std::priority_queue<GraphNodePtr, std::vector<GraphNodePtr>, std::greater<GraphNodePtr>> queue;
    nodes[from].minimum_path = 0;
    queue.emplace(GraphNodePtr(nodes[from], from));

    while (!queue.empty()) {
        GraphNodePtr node_ptr = queue.top();
        queue.pop();
        if (node_ptr.isValid()) {
            GraphNode &current_node = *(node_ptr.node);
            for (GraphEdge edge : current_node.edges) {
                GraphNode &child_node = nodes[edge.to];
                int new_path = current_node.minimum_path + edge.weight_edge;
                if (!child_node.considered && child_node.minimum_path > new_path) {
                    // релаксируем вершину.
                    child_node.minimum_path = new_path;
                    queue.emplace(GraphNodePtr(child_node, edge.to));
                }
            }
            current_node.considered = true;
        } // если указатель невалиден - ничего не делаем.
    }
};


int main() {
    // Формат ввода
    // Первая строка содержит число N – количество городов.
    size_t number_of_cities = 0;
    std::cin >> number_of_cities;
    // Вторая строка содержит число M - количество дорог.
    size_t number_of_roads = 0;
    std::cin >> number_of_roads;

    ListGraph list_graph(number_of_cities);

    // Каждая следующая строка содержит описание дороги (откуда, куда, время в пути).
    for (int i = 0; i < number_of_roads; ++i) {
        int from = -1, to = -1, time = 0; // чтоб упало при некорректном вводе.
        std::cin >> from >> to >> time;
        list_graph.AddEdge(from, to, time);
    }
    // Последняя строка содержит маршрут (откуда и куда нужно доехать). */
    int from = -1, to = -1;
    std::cin >> from >> to;
    std::cout << list_graph.LenthOfShortestPaths(from, to) << std::endl;
    return 0;
}