// https://contest.yandex.ru/contest/7930/problems/2_2/
/* Обход дерева в порядке pre-order
 * Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * То есть, при добавлении очередного числа K в дерево с корнем root, если root → Key ≤ K,
 * то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Выведите элементы в порядке pre-order (сверху вниз).
 * Рекурсия запрещена.
 */

/* Ввод

10
2
5
6
4
7
8
9
3
1
10

построенное дерево:

      2
     / \
    1   5
       / \
      4   6
     /     \
    3       7
             \
              8
               \
                9
                 \
                 10
Вывод

2 1 5 4 3 6 7 8 9 10

Ввод

10
5
9
4
3
7
2
6
10
1
8

Вывод

5 4 3 2 1 9 7 6 8 10

Ввод

10
1
5
10
2
4
3
8
9
7
6

Вывод

1 5 2 4 3 10 8 7 6 9

*/

#include <iostream>
#include <stack>

namespace my {

    template<typename Stored>
    class Node {
    public:
        Stored *value;
        Node *left;
        Node *right;

        Node() = delete;

        Node(Stored *init_ptr) : left(nullptr), right(nullptr) {
            value = init_ptr;
        }

        ~Node() {
            delete left;
            delete right;
            delete value;
        }
    };

    template<typename Stored, typename Less = std::less<Stored>>
    class UnbalancedTree {
    private:
        Node<Stored> *root = nullptr;
        u_int length = 0;
        Less less;

//        class iterator {
//        public:
//            iterator(DataType * ptr): ptr(ptr){}
//            iterator operator++() { ++ptr; return *this; }
//            bool operator!=(const iterator & other) { return ptr != other.ptr; }
//            const DataType& operator*() const { return *ptr; }
//        private:
//            DataType* ptr;
//        };
    public:
        UnbalancedTree() {

        }

        ~UnbalancedTree() {
            delete root;
        }

        void add(Stored &value);

        void add(Stored *value);

        u_int len() {
            return length;
        }

        void print();

//        iterator begin() const;
//
//        iterator end() const;
    };


    template<typename Stored, typename Less>
    void UnbalancedTree<Stored, Less>::add(Stored &value) {
        auto tmp = new Stored(value);
        add(tmp);
    }

    template<typename Stored, typename Less>
    void UnbalancedTree<Stored, Less>::add(Stored *value) {
        // каждый элемен имеет указатели на первого или левого ребёнка.
        // current_pointer - указатель на текущий указатель внутри Node.
        // спускаемся по дереву, пока не наткнёмся на nullptr. Туда и запишем новое значение.
        Node<Stored> **current_pointer = &root;
        while (*current_pointer != nullptr) {
            if (less(*value, *(**current_pointer).value)) {
                current_pointer = &(**current_pointer).left;
            } else {
                current_pointer = &(**current_pointer).right;
            }
        }
        *current_pointer = new Node<Stored>(value);
        ++length;
    }

    template<typename Stored, typename Less>
    void UnbalancedTree<Stored, Less>::print() {
        // стек, хранящий текущее звено и состояние обхода
        enum State {        // последнее совершённое над Node действие
            PutNode,       // положили в стек
            OutputValue,   // вывели хранимое значение
            OutputLeft,    // вывели левое дерево
            OutputRight    // вывели правое дерево
        };

        auto node_stack = std::stack<std::pair<State, Node<Stored> *>>();
        if (root != nullptr) {
            node_stack.push({PutNode, root});
        };
        while (node_stack.size() != 0) {
            auto &state_and_node = node_stack.top();
            switch (state_and_node.first) {
                case PutNode: {
                    state_and_node.first = OutputValue;
                    std::cout << *(state_and_node.second->value) << " ";
                }
                    break;
                case OutputValue: {
                    state_and_node.first = OutputLeft;
                    if (state_and_node.second->left != nullptr) { // есть левое поддерево
                        node_stack.push({PutNode, state_and_node.second->left});
                    }
                }
                    break;
                case OutputLeft: {
                    state_and_node.first = OutputRight;
                    if (state_and_node.second->right != nullptr) { // есть правое поддерево
                        node_stack.push({PutNode, state_and_node.second->right});
                    }
                }
                    break;
                case OutputRight: {
                    node_stack.pop();
                }
                    break;
            }
        }
    };

}


int main() {
    my::UnbalancedTree<int> tree;
    int length = 0;
    std::cin >> length;
    for (int i = 0; i < length; ++i) {
        int node = 0;
        std::cin >> node;
        tree.add(node);
    }
    tree.print();
    return 0;
}