// https://contest.yandex.ru/contest/7930/problems/3_1/
// https://drive.google.com/file/d/1x5xFk98JyC5bGDOIn13XiecojrJnA0Le/view
// http://e-maxx.ru/algo/treap
// https://neerc.ifmo.ru/wiki/index.php?title=Декартово_дерево
/*
Декартово дерево. Разница глубин.
1 секунда, 64Mb, стандартный ввод/вывод.

Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
Добавление узла в декартово дерево выполняйте следующим образом:
При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
а во втором больше или равны x.
Получившиеся два дерева сделайте дочерними для нового узла (x, y).
Новый узел вставьте на место узла P.

Построить также наивное дерево поиска по ключам Xi.
То есть, при добавлении очередного числа K в наивное дерево с корнем root,
если root→Key ≤ K,
    то узел K добавляется в правое поддерево root;
иначе
    в левое поддерево root.

Вычислить разницу глубин наивного дерева поиска и декартового дерева.
Разница может быть отрицательна, необходимо вывести модуль разности.

Формат ввода
В первой строке записано число N — количество узлов.
В каждой следующей строке записаны два числа Xi, Yi — ключ и приоритет узла.

Формат вывода
Неотрицательное целое число.

Пример 1
Ввод

10
5 11
18 8
25 7
50 12
30 30
15 15
20 10
22 5
40 20
45 9

Вывод

2

Пример 2
Ввод

10
38 19
37 5
47 15
35 0
12 3
0 42
31 37
21 45
30 26
41 6

Вывод

2

*/

#include <iostream>
#include <exception>

namespace my {
    class Knot {
    public:
        int key;        // X
        int priority;   // Y
        Knot *left;
        Knot *right;

        Knot() = delete;

        Knot(int &key, int &priority) :
                key(key),
                priority(priority),
                left(nullptr),
                right(nullptr) {
        };

        ~Knot() {
            delete left;
            delete right;
        }

        int height() const;

    };

    // рекурсивно вычисляет высоту дерева
    // пустое поддерево имеет высоту 0
    // вершина имеет высоту наибольшего из поддеревьев + 1.
    int Knot::height() const {
        int left_height = (left != nullptr) ? left->height() : 0;
        int right_height = (right != nullptr) ? right->height() : 0;
        int real_height = (left_height > right_height) ? left_height + 1 : right_height + 1;
        return real_height;
    };

    // Наивное дерево.
    class NaiveTree {
    private:
        Knot *root;

    public:
        NaiveTree() : root(nullptr) {};

        ~NaiveTree() {
            delete root;
        };

        void add(int &key, int &priority);

        int height() const;

    };

    // наивное добавление, не использует priority.
    void NaiveTree::add(int &key, int &priority) {
        Knot *current = root;
        Knot **parent = &root;
        while (current != nullptr) {
            if (current->key > key) {
                parent = &current->left;
                current = current->left;
            } else if ((**parent).key < key) {
                parent = &current->right;
                current = current->right;
            } else { // if equal
                throw std::exception();
            }
        }
        *parent = new Knot(key, priority);
    }

    // высота пустого дерева = 0
    // высота непутого дерева = высоте корневой вершины.
    int NaiveTree::height() const{
        int tree_height = (root != nullptr)? root->height() : 0;
        return tree_height;
    }

    class CartesianTree {
    private:
        Knot *root;

        void split(Knot *currentNode, int key, Knot *&left, Knot *&right);

    public:
        CartesianTree() : root(nullptr) {};

        void add(int &key, int &priority);

        int height() const;

    };

    // Разрезание декартового дерева по ключу.
    void CartesianTree::split(Knot *currentNode, int key, Knot *&left, Knot *&right) {
        if (currentNode == nullptr) {
            left = nullptr;
            right = nullptr;
        } else if (currentNode->key <= key) {
            split(currentNode->right, key, currentNode->right, right);
            left = currentNode;
        } else {
            split(currentNode->left, key, left, currentNode->left);
            right = currentNode;
        }
    }

    // При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
    // Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x,
    // а во втором больше или равны x. Получившиеся два дерева сделайте дочерними для нового узла (x, y).
    // Новый узел вставьте на место узла P.
    void CartesianTree::add(int &key, int &priority) {
        // parent всегда валидный указатель на то место, куда надо приделать новый элемент.
        Knot **parent = &root;
        // current указатель на вершину дерева, которое надо разделить по ключу key. дерево может быть пустым.
        Knot *current = root;
        while (current != nullptr && current->priority > priority) {
            if (current->key != key) {
                if (current->key > key) {
                    parent = &current->left;
                    current = current->left;
                } else { // if (current->key < key) {
                    parent = &current->right;
                    current = current->right;
                }
            } else {
                std::exception();
            }
        }
        *parent = new Knot(key, priority);
        split(current, key, (*parent)->left, (*parent)->right);
    }

    int CartesianTree::height() const{
        int tree_height = (root != nullptr)? root->height() : 0;
        return tree_height;
    }
}

int main() {
    my::NaiveTree naive_tree;
    my::CartesianTree cartesian_tree;
    int in_lenth = 0;
    std::cin >> in_lenth;
    for (int i = 0; i < in_lenth; ++i) {
        int tmp_key = 0, tmp_priority = 0;
        std::cin >> tmp_key >> tmp_priority;
        naive_tree.add(tmp_key, tmp_priority);
        cartesian_tree.add(tmp_key, tmp_priority);
    }
    std::cout << abs(cartesian_tree.height() - naive_tree.height());
    return 0;
}