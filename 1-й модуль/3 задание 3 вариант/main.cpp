// https://contest.yandex.ru/contest/7595/problems/3_3/
// Очередь с помощью стека
// стандартный ввод, стандартный вывод
// Реализовать очередь с помощью двух стеков.
//     ↓                       ↓   ↑                       ↑
//    ┃↓┃ ┃ ┃     ┃ ┃ ┃ ┃     ┃↓┃ ┃↑┃     ┃ ┃ ┃ ┃     ┃ ┃ ┃↑┃     ┃ ┃ ┃ ┃
//    ┃↓┃ ┃ ┃     ┃ ┃ ┃ ┃     ┃ ┃ ┃↑┃     ┃ ┃ ┃ ┃     ┃ ┃ ┃ ┃     ┃ ┃ ┃ ┃
//    ┃ ┃ ┃ ┃ →→→ ┃3┃↷┃ ┃ →→→ ┃ ┃ ┃1┃ →→→ ┃ ┃ ┃ ┃ →→→ ┃ ┃ ┃ ┃ →→→ ┃ ┃ ┃ ┃
//    ┃ ┃ ┃ ┃     ┃2┃↷┃ ┃     ┃ ┃ ┃2┃     ┃5┃↷┃ ┃     ┃ ┃ ┃4┃     ┃ ┃ ┃ ┃
//    ┃ ┃ ┃ ┃     ┃1┃↷┃ ┃     ┃ ┃ ┃3┃     ┃4┃↷┃ ┃     ┃ ┃ ┃5┃     ┃ ┃ ┃ ┃
//    ┗━┛ ┗━┛     ┗━┛ ┗━┛     ┗━┛ ┗━┛     ┗━┛ ┗━┛     ┗━┛ ┗━┛     ┗━┛ ┗━┛
//    in  out     in  out     in  out     in  out     in  out     in  out
// https://stackoverflow.com/questions/69192/how-to-implement-a-queue-using-two-stacks
// Использовать стек, реализованный с помощью динамического буфера.
//        ← in_lenght: индекс следующего после последнего выделенного
//    ┃ ┃
//    ┃ ┃ ← in_used:   индекс следующего, после последнего добавленного
//    ┃3┃
//    ┃2┃
//    ┃1┃
//    ┃0┃ ← in_stack:  указатель на нулевой элемент динамического буффера.
//    ┗━┛
//    in
// Обрабатывать команды push back и pop front.
//     кладём push_back`ом в in при любом состоянии стеков
//     вытаскиваем pop_front`ом только при условии, что из in_stack уже всё перенесли в out_stack.
//         переносим, если не так.
//     переносим только в случае, когда out_stack пуст
// Формат ввода
//     В первой строке количество команд n. n ≤ 1000000.
//     Каждая команда задаётся как 2 целых числа: a b.
//     a = 2 - pop front
//     a = 3 - push back
//     Если дана команда pop front, то число b - ожидаемое значение.
//     Если команда pop front вызвана для пустой структуры данных, то ожидается “-1”.
// Формат вывода
//     Требуется напечатать YES - если все ожидаемые значения совпали.
//     Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.
// Пример
/* Ввод
3
3 44
3 50
2 44
// YES
2
2 -1
3 10
// Yes
2
3 44
2 66
// NO */



#include <cstring>
#include <stdexcept>
#include <iostream>

#define INIT_SIZE 32

template<typename Stored>
class Stack {
private:
    Stored *array;
    u_int used;
    u_int length;

    void resize();

public:
    Stack();

    ~Stack();

    void push(Stored &element); // помещение в стек
    Stored pop(); // вытаскивание из стека

    u_int len() const {
        return used;
    }
};

template<typename Stored>
Stack<Stored>::Stack() {
    array = static_cast<Stored *>(operator new(sizeof(Stored) * INIT_SIZE));
    length = INIT_SIZE;
    used = 0;
}

template<typename Stored>
Stack<Stored>::~Stack() {
    operator delete(array);
};

template<typename Stored>
void Stack<Stored>::resize() {
    // выделение памяти без вызова конструкторов
    auto new_array = static_cast<Stored *>(operator new(sizeof(Stored) * length * 3));
    std::memcpy(new_array, array, used * sizeof(Stored));
    operator delete(array);
    array = new_array;
    length *= 3;
}

template<typename Stored>
void Stack<Stored>::push(Stored &element) {
    if (used >= length) { // если места нет
        resize();
    }
    array[used++] = element;
};

template<typename Stored>
Stored Stack<Stored>::pop() {
    if (used != 0) {
        return std::move(array[--used]);
    } else {
        throw std::out_of_range("stack is empty");
    }
};

template<typename Stored>
class Queue {
private:
    // буфер, в который записываем push_back`ом
    Stack<Stored> in_stack;
    // буфер, из которого вытаскиваем pop_front`ом
    Stack<Stored> out_stack;
public:
    Queue() = default;

    ~Queue() = default;

private:
    // перекладывает всё из in_stack в out_stack
    void shift_all();

public:
    void push_back(Stored &element);

    Stored pop_front();

    u_int len () const{
        return in_stack.len() + out_stack.len();
    };
};

// перекладывает всё из in_stack в out_stack
template<typename Stored>
void Queue<Stored>::shift_all() {
    while (in_stack.len() != 0) {
        auto tmp = in_stack.pop();
        out_stack.push(tmp);
    }
};

template<typename Stored>
void Queue<Stored>::push_back(Stored &element) {
    in_stack.push(element);
};

template<typename Stored>
Stored Queue<Stored>::pop_front() {
    if (out_stack.len() != 0) {
        return out_stack.pop();
    } else {
        if (in_stack.len() != 0) {
            shift_all();
            return out_stack.pop();
        } else {
            return Stored(-1);
            // throw std::out_of_range("queue is empty");
        }
    }
};


int main() {

    Queue<int> main_queue;
    u_int number_of_iterations;
    int a_command_code;
    int b_value;

    std::cin >> number_of_iterations;
    for (u_int i = 0; i < number_of_iterations; ++i) {
        std::cin >> a_command_code >> b_value;
        if (a_command_code == 2) {
            if (b_value != -1) {
                if (main_queue.pop_front() == b_value) {
                    // всё нормально, значения входное и в очереди совпали
                } else {
                    std::cout << "NO" << std::endl;
                    return 0;
                }
            } else {
                if (main_queue.len() == 0) {
                    // всё нормально, значения и не должно быть
                } else {
                    std::cout << "NO" << std::endl;
                    return 0;
                }
            }
        } else if (a_command_code == 3) {
            main_queue.push_back(b_value);
        } else {
            std::cout << "NO" << std::endl;
            return 0;
        }
    }
    std::cout << "YES" << std::endl;
    return 0;
}
