// https://contest.yandex.ru/contest/7930/problems/4_1/
/* Задача 4_1. Использование АВЛ-дерева, Солдаты.
1 секунда, 20Mb, стандартные ввод и вывод.

В одной военной части решили построить в одну шеренгу по росту.
Так как часть была далеко не образцовая, то солдаты часто приходили не вовремя,
а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту –
сначала самые высокие, а в конце – самые низкие.
За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.
Требуемая скорость выполнения команды - O(log n).

Формат ввода
Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000).
В каждой следующей строке содержится описание команды:
    число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) и
    число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя.
Солдаты в строе нумеруются с нуля.

Формат вывода
На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
на которую должен встать этот солдат (все стоящие за ним двигаются назад).

Пример 1
Ввод

5
1 100
1 200
1 50
2 1
1 150


Вывод

0
0
2
1

Пример 2
Ввод
 
15
1 41
1 18467
2 0
1 26500
1 19169
2 1
1 11478
1 29358
2 2
1 24464
1 5705
2 0
1 23281
1 16827
2 1


Вывод

0
0
0
1
1
0
2
3
2
3

*/

#include <stdexcept>
#include <iostream>

namespace my {
    class Knot {
    public:
        int key;
        int saved_height;
        int number_of_left; // количество вершин в левом поддереве
        int number_of_right; // количество вершин в правом поддереве
        Knot *left;
        Knot *right;

        Knot() = delete;

        explicit Knot(int &key) :
                key(key),
                saved_height(1),
                number_of_left(0),
                number_of_right(0),
                left(nullptr),
                right(nullptr) {
        };

        ~Knot() {
            delete left;
            delete right;
        }

        // Определим три вспомогательные функции, связанные с высотой.
        // Первая является оберткой для поля saved_height,
        // она может работать и с нулевыми указателями (с пустыми деревьями):
        int height() {
            return (this != nullptr) ? saved_height : 0;
        }

        // Вторая вычисляет balance_factor заданного узла (и работает только с ненулевыми указателями):
        int balance_factor() {
            return right->height() - left->height();
        }

        // Третья функция восстанавливает корректное значение поля height заданного узла
        // (при условии, что значения этого поля в правом и левом дочерних узлах являются корректными):
        void fix_height() {
            int height_left = left->height();
            int height_right = right->height();
            saved_height = ((height_left > height_right) ? height_left : height_right) + 1;
        }
        // Заметим, что все три функции являются нерекурсивными, время работы константно.

        // Определим две вспомогательные функции, связанные с количеством хранимых элементов.
        // Первая является оберткой для полей number_of_left, number_of_right(0)
        // она может работать и с нулевыми указателями (с пустыми деревьями):
        int number_of_nodes() {
            return (this != nullptr) ? (number_of_right + number_of_left + 1) : 0;
        }

        // Третья функция восстанавливает корректное значение поля height заданного узла
        // (при условии, что значения этого поля в правом и левом дочерних узлах являются корректными):
        void fix_number_of_nodes() {
            number_of_left = left->number_of_nodes();
            number_of_right = right->number_of_nodes();
        }
        // Заметим, что обе функции являются нерекурсивными, время работы константно.

        // для удобства сделаем распечатку структуры дерева.
        void _print(std::ostream &os) const {
            os << "{\"key\": " << key
               << ", \"saved_height\": " << saved_height
               << ", \"on_left\": " << number_of_left
               << ", \"on_right\": " << number_of_right << "}\n";
        }
    };

    class AVL_Tree {
    private:
        Knot *root;

    public:
        AVL_Tree() : root(nullptr) {};

        /* Вставка нового ключа с возвратом индекса, на который вставлено значение*/
        int add(int key);

        /* Поиск узла по индексу */
        Knot *find_by_index(int target_index);

        /* Удаление по ключу.*/
        void dell_by_key(int key);

        /* Удаление по индексу.*/
        void dell_by_index(int position);

        friend std::ostream &operator<<(std::ostream &os, const AVL_Tree &tree) {
            tree._print(os, tree.root, 0);
        };

    private:
        Knot *rotate_right(Knot *y);

        Knot *rotate_left(Knot *x);

        Knot *balance(Knot *x);

        Knot *insert(Knot *x, int key, int *position_counter);

//      Knot *find_min(Knot *x);

//      Knot *remove_min(Knot *x);

        struct Tuple {
            Knot *min; // указатель на минимальный элемент - 1 раз устанавливается
            Knot *subtree;
        };

        Tuple remove_and_return_min(Knot *x);

        Knot *remove_by_key(Knot *x, int key);

        void _print(std::ostream &os, Knot *x, int depth) const;

        Knot *remove_by_index(Knot *x, int target_index, int buffer_index);
    };

    /*
    Балансировка узлов
    В процессе добавления или удаления узлов в АВЛ-дереве возможно возникновение ситуации,
    когда balance factor некоторых узлов оказывается равными 2 или -2, возникает расбалансировка поддерева.
    Для выправления ситуации применяются повороты вокруг тех или иных узлов дерева.
    Простой поворот вправо (влево) производит следующую трансформацию дерева:

                 y  <--левый-правый-->  x
                / \                    / \
               /   \                  /   \
              x     c                a     y
             / \   /=\              /=\   / \
            /   \                        /   \
           a     b                      b     c
          /=\   /=\                    /=\   /=\

                    A < x < B < y < c

    Код, реализующий правый поворот, выглядит следующим образом (как обычно, каждая функция, изменяющая дерево,
    возвращает новый корень полученного дерева): */

    Knot *AVL_Tree::rotate_right(Knot *y) { // правый поворот вокруг y
        Knot *x = y->left;
        y->left = x->right;
        x->right = y;
        y->fix_height();
        x->fix_height();
        y->fix_number_of_nodes();
        x->fix_number_of_nodes();
        return x;
    }

    /* Левый поворот является симметричной копией правого: */

    Knot *AVL_Tree::rotate_left(Knot *x) { // левый поворот вокруг q
        Knot *y = x->right;
        x->right = y->left;
        y->left = x;
        x->fix_height();
        y->fix_height();
        x->fix_number_of_nodes();
        y->fix_number_of_nodes();
        return y;
    }

    // Код, выполняющий балансировку, сводится к проверке условий и выполнению поворотов:
    Knot *AVL_Tree::balance(Knot *x) // балансировка узла x
    {
        x->fix_height();
        x->fix_number_of_nodes();
        if (x->balance_factor() == 2) {
            if (x->right->balance_factor() < 0)
                x->right = rotate_right(x->right);
            return rotate_left(x);
        }
        if (x->balance_factor() == -2) {
            if (x->left->balance_factor() > 0)
                x->left = rotate_left(x->left);
            return rotate_right(x);
        }
        return x; // балансировка не нужна
    }
    /* Описанные функции поворотов и балансировки также не содержат ни циклов, ни рекурсии,
    а значит выполняются за постоянное время, не зависящее от размера АВЛ-дерева. */

    /* Вставка ключей

    Вставка нового ключа в АВЛ-дерево выполняется, так же, как это делается в простых деревьях поиска:
    спускаемся вниз по дереву, выбирая правое или левое направление движения
    в зависимости от результата сравнения ключа в текущем узле и вставляемого ключа.
    Единственное отличие заключается в том, что при возвращении из рекурсии
    (после того, как ключ вставлен либо в правое, либо в левое поддерево, и это дерево сбалансировано)
    выполняется балансировка текущего узла. Строго доказывается, что возникающий при такой вставке дисбаланс
    в любом узле по пути движения не превышает двух,
    а значит применение вышеописанной функции балансировки является корректным.

    Важно, что балансировка дерева не нарушает позицию в массиве (порядковую статистику каждого элемента)
    Можно считать новый индекс и до того, как мы окончательно добавили новый узел.
    если идём налево при выборе ничего не прибавляем
    если идём направо - прибавляем количество элементов слева + 1
    */
    Knot *AVL_Tree::insert(Knot *x, int key, int *position_counter) { // вставка ключа k в дерево с корнем x
        if (x == nullptr) {
            return new Knot(key);
        }
        if (key < x->key) {
            *position_counter += x->number_of_right + 1;
            x->left = insert(x->left, key, position_counter);
        } else {
            x->right = insert(x->right, key, position_counter);
        }
        return balance(x);
    }

    int AVL_Tree::add(int key) {
        int counter = 0;
        root = insert(root, key, &counter);
        return counter;
    };

    /* Поиск ключа по индексу */
    Knot *AVL_Tree::find_by_index(int target_index) {
        // проще написать дерево если индексом будет кличество элементов в правом поддереве + 1
        // это значтит, что самый правый элемент будет 1-м.
        target_index += 1;

        Knot *current_knot = root;

        // количество элементов в правых поддеревьях родительских элементов +
        // количество родителей, которых мы оставили справа.
        int buffer_index = 0;

        while (true) {
            if (current_knot == nullptr) {
                throw std::exception(); // нам передали индекс за пределами дерева.
            }
            int current_index = buffer_index + current_knot->number_of_right + 1; // индекс текущего узла
            if (current_index == target_index) {
                return current_knot;
            } else if (current_index < target_index) {
                buffer_index += current_knot->number_of_right + 1;
                current_knot = current_knot->left;
            } else { // if (current_index > target_index)
                current_knot = current_knot->right;
            }

        }
    }

    /* Удаление ключей
    С удалением узлов из АВЛ-дерева, все не так просто, как с рандомизированными деревьями поиска.
    Идея следующая: находим узел x с заданным ключом k (если не находим, то делать ничего не надо),
    в правом поддереве находим узел min с наименьшим ключом и заменяем удаляемый узел p на найденный узел min.
    Если у найденного узела x нет правого поддерева, то по свойству АВЛ-дерева слева у этого узла может
    быть только один единственный дочерний узел (дерево высоты 1), либо узел p вообще лист.
    В обоих этих случаях надо просто удалить узел p и вернуть в качестве результата
    указатель на левый дочерний узел узла x.
    Пусть теперь правое поддерево у x есть. Находим минимальный ключ в этом поддереве.
    По свойству двоичного дерева поиска этот ключ находится в конце левой ветки, начиная от корня дерева.
    Применяем рекурсивную функцию: */

    // поиск узла с минимальным ключом в дереве x - самого левого в поддереве
//  Knot *AVL_Tree::find_min(Knot *x) {
//      return (x->left != nullptr) ? find_min(x->left) : x;
//  }

    /* Еще одна служебная функция у нас будет заниматься удалением минимального элемента из заданного дерева.
    По свойству АВЛ-дерева у минимального элемента справа либо подвешен единственный узел, либо там пусто.
    В обоих случаях надо просто вернуть указатель на правый узел и по пути назад (при возвращении из рекурсии)
    выполнить балансировку. Сам минимальный узел не удаляется - он нам еще пригодится. */

//  Knot *AVL_Tree::remove_min(Knot *x) { // удаление узла с минимальным ключом из дерева x
//      if (x->left == nullptr) {
//          return x->right;
//      } else {
//          x->left = remove_min(x->left);
//          return balance(x);
//      }
//  }

    /* Поиск минимального узла и его извлечение, можно реализовать в одной функции, при этом придется
    решать (не очень сложную) проблему с возвращением из функции пары указателей. Зато можно сэкономить на одном
    проходе по правому поддереву. */
    AVL_Tree::Tuple AVL_Tree::remove_and_return_min(Knot *x) {
        // передали поддерево, надо вернуть это поддерево с удалённым элементом и минимальный элемент.
        // нерекурсивный случай - мы нашли минимальный элемент
        if (x->left == nullptr) {
            return Tuple{x, x->right};
        } else {
            Tuple tuple = remove_and_return_min(x->left);
            x->left = tuple.subtree;
            return Tuple{tuple.min, balance(x)};
        }
    }

    /* Теперь все готово для реализации удаления ключа из АВЛ-дерева.
    Сначала находим нужный узел, выполняя те же действия, что и при вставке ключа: */

    Knot *AVL_Tree::remove_by_key(Knot *x, int key) { // удаление ключа key из дерева x
        if (x == nullptr) {
            throw std::exception(); // нам передали ключ за пределами дерева.
        }
        if (key < x->key) {
            x->left = remove_by_key(x->left, key);
        } else if (key > x->key) {
            x->right = remove_by_key(x->right, key);
        }
            /* Как только ключ key найден, переходим к плану Б:
            запоминаем корни q и r левого и правого поддеревьев узла p;
            удаляем узел p;
            если правое поддерево пустое,
                то возвращаем указатель на левое поддерево;
            если правое поддерево не пустое,
                то находим там минимальный элемент min,
                потом его извлекаем оттуда,
                слева к min подвешиваем q,
                справа — то, что получилось из r,
                возвращаем min после его балансировки. */
        else { //  k == p->key
            Knot *q = x->left;
            Knot *r = x->right;
            // для избежания каскадного удаления поддеревьев при вызове деструктора узла.
            x->left = nullptr;
            x->right = nullptr;
            delete x;
            if (r == nullptr) {
                return q;
            }
            Tuple min_and_subtree = remove_and_return_min(r);
            min_and_subtree.min->right = min_and_subtree.subtree;
            min_and_subtree.min->left = q;
            return balance(min_and_subtree.min);
        }
        // При выходе из рекурсии не забываем выполнить балансировку
        return balance(x);
    }

    void AVL_Tree::dell_by_key(int key) {
        root = remove_by_key(root, key);
    };

    /* Совмещая логику отлаженных по отдельности функций remove_by_key и find_by_index
     * напишем функцию удаления по индексу из дерева x*/
    Knot *AVL_Tree::remove_by_index(Knot *x, int target_index, int buffer_index) {
        if (x == nullptr) {
            throw std::exception(); // нам передали индекс за пределами дерева.
        }
        int current_index = buffer_index + x->number_of_right + 1; // индекс текущего узла
        if (current_index < target_index) {
            x->left = remove_by_index(x->left, target_index, buffer_index + x->number_of_right + 1);
        } else if (current_index > target_index) {
            x->right = remove_by_index(x->right, target_index, buffer_index);
        }
            /* Как только индекс найден, переходим к плану Б:
            запоминаем корни q и r левого и правого поддеревьев узла p;
            удаляем узел p;
            если правое поддерево пустое,
                то возвращаем указатель на левое поддерево;
            если правое поддерево не пустое,
                то находим там минимальный элемент min,
                потом его извлекаем оттуда,
                слева к min подвешиваем q,
                справа — то, что получилось из r,
                возвращаем min после его балансировки. */
        else {
            Knot *q = x->left;
            Knot *r = x->right;
            // для избежания каскадного удаления поддеревьев при вызове деструктора узла.
            x->left = nullptr;
            x->right = nullptr;
            delete x;
            if (r == nullptr) {
                return q;
            }
            Tuple min_and_subtree = remove_and_return_min(r);
            min_and_subtree.min->right = min_and_subtree.subtree;
            min_and_subtree.min->left = q;
            return balance(min_and_subtree.min);
        }
        // При выходе из рекурсии не забываем выполнить балансировку
        return balance(x);
    }

    void AVL_Tree::dell_by_index(int position) {
        root = remove_by_index(root, position + 1, 0);
    }

    /*Очевидно, что операции вставки и удаления (а также более простая операция поиска)
    выполняются за время пропорциональное высоте дерева, т.к. в процессе выполнения этих операций производится
    спуск из корня к заданному узлу, и на каждом уровне выполняется некоторое фиксированное число действий. А в
    силу того, что АВЛ-дерево является сбалансированным, его высота зависит логарифмически от числа узлов. Таким
    образом, время выполнения всех трех базовых операций гарантированно логарифмически зависит от числа узлов дерева.*/

    void AVL_Tree::_print(std::ostream &os, Knot *x, int depth) const {
        if (x->left != nullptr) {
            _print(os, x->left, depth + 1);
        }
        for (int i = 0; i < depth; ++i) {
            os << "\t";
        }
        x->_print(os);
        if (x->right != nullptr) {
            _print(os, x->right, depth + 1);
        }
    }
}

int main() {
    my::AVL_Tree avl_tree;
    // Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000).
    int number_of_instruction = 0;
    std::cin >> number_of_instruction;
    int command_type = 0;
    int height_or_index = 0;
    for (int i = 0; i < number_of_instruction; ++i) {
        // В каждой следующей строке содержится описание команды:
        std::cin >> command_type >> height_or_index;
        // число 1 и X если солдат приходит в строй (X – рост солдата, натуральное число до 100 000 включительно) и
        if (command_type == 1) {
            int position = avl_tree.add(height_or_index);
            // На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции,
            // на которую должен встать этот солдат (все стоящие за ним двигаются назад).
            std::cout << position << "\n";
            // число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя.
            // Солдаты в строе нумеруются с нуля.
        } else if (command_type == 2) {
            avl_tree.dell_by_index(height_or_index);
        }// else if (command_type == 3) {
//            std::cout << avl_tree;
//        } else if (command_type == 4) {
//            avl_tree.dell_by_key(height_or_index);
//        } else if (command_type == 5) {
//            std::cout << avl_tree.find_by_index(height_or_index)->key << "\n";
//        }
    }
    return 0;
}