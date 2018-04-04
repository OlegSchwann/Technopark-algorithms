// https://contest.yandex.ru/contest/7595/problems/4_2/
// Быстрое сложение
// стандартный ввод, стандартный вывод
// Для сложения чисел используется старый компьютер.
// Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
// Для нахождения суммы чисел в зависимости от порядка вычислений требуется разное время.
// ((1+2)+3) -> 1+2 + 3+3 =  9
// ((1+3)+2) -> 1+3 + 4+2 = 10
// ((2+3)+1) -> 2+3 + 5+1 = 11
// Требуется написать программу, которая определяет минимальное время,
// достаточное для вычисления суммы заданного набора чисел.
// Решение задачи предполагает использование кучи, реализованной в виде класса.
// Вначале вводится n - количество чисел.
// Затем вводится n строк - значения чисел не превосходят signed int.
// Выводится натуральное число - минимальное время.
// Ввод
// 5             5
// 5 2 3 4 6     3 7 6 1 9
// Вывод
// 45            56

#include <malloc.h>
#include <algorithm>
#include <iostream>

// класс кучи, шаблонизируемый операцией меньше
// в верхушке кучи находится минимальный элемент
// поддерживаемые операции:
// вставка с сохранением целостности
// вытаскивание наименьшего с удалением из кучи с сохранением целостности
// размер.

#define INIT_SIZE 32 // первоначальный размер кучи

// Обычно компаратор передают в конструкторе и держат в объекте, чтобы не создавать его каждый раз.
// Кроме того, это позволяет параметризовать компаратор снаружи очереди.
// В остальном - правильно.

template<typename Stored, typename Less = std::less<Stored>>
class PriorityQueue {
private:
    Stored *array;        // array
    unsigned int used;    // ↓
    unsigned int capasity;// 0 1 2 3 4 5 □ □ □ □     массив хранимых значений
    //             ↑       ↑
    //             used    capasity
    void resize();

public:
    PriorityQueue();

    ~PriorityQueue();

    void push(Stored &input_value);

    Stored pop();

    inline unsigned int size() const {
        return used;
    };

};

template<typename Stored, typename Less>
std::istream& operator>> (std::istream &in_stream, PriorityQueue <Stored, Less>&heap){
    Stored in_value;
    in_stream >> in_value;
    heap.push(in_value);
    return in_stream;
}

template<typename Stored, typename Less>
PriorityQueue<Stored, Less>::PriorityQueue() {
    array = (Stored *) malloc(sizeof(Stored) * INIT_SIZE);
    used = 0;
    capasity = INIT_SIZE;
}

template<typename Stored, typename Less>
PriorityQueue<Stored, Less>::~PriorityQueue() {
    for (unsigned int i = 0; i < used; ++i) {
        // вызываем деструктор освобождая память динамических полей того, что лежит в массиве
        // не трогая сам массив.
        array[i].~Stored();
    };
    free(array);
}

// увеличивает в 2 раза вместительность кучи
template<typename Stored, typename Less>
void PriorityQueue<Stored, Less>::resize() {
    auto new_capacity = capasity * 2;
    auto new_array = (Stored *) realloc(array, new_capacity * sizeof(Stored));
    if (new_array != nullptr) {
        array = new_array;
        capasity = new_capacity;
    } else {
        throw std::bad_alloc();
    }
}

// положить элемент в кучу.
// увеличиваем массив на 1, кладём в него элемент в конец с вызовом констуктора копирования
// если родитель меньше - всё хорошо
// пока родитель существует и родитель больше
//     меняем с родителем,
//     меняем индекс на индекс родителя

// A[0] – элемент в корне,
// предок  элемента A[i] – элемент  A[(i – 1)/2].
template<typename Stored, typename Less>
void PriorityQueue<Stored, Less>::push(Stored &input_value) {
    Less less;
    if (used == capasity) { // нет свободного места
        resize();
    }
    array[used] = input_value;
    for (unsigned int i = used; i > 0;) {
        auto parent_i = (i - 1) / 2;
        if (less(array[i], array[parent_i])) {
            std::swap<Stored>(array[parent_i], array[i]);
            i = parent_i;
        } else {
            break;
        }
    }
    ++used; // теперь вновь указываем на следующий после последнего элемент
};

// вытаскивание самого маленького элемента наружу
// A[0] – элемент в корне,
// потомки элемента A[i] – элементы A[2i + 1] и A[2i + 2].
// меняем последний и первый элемент в массиве
// уменьшаем i - при этом i указывает на то, что нужно вернуть.
// для цепочки от корня:
// Если i-й элемент меньше, чем его сыновья, всё хорошо.
// В противном случае меняем местами i-й элемент с наименьшим из его сыновей,
// Меняем индекс на индекс этого сына.
template<typename Stored, typename Less>
Stored PriorityQueue<Stored, Less>::pop() {
    Less less;
    --used;
    std::swap<Stored>(array[0], array[used]);
    for (unsigned int i = 0; i < used;) {
        auto left_i = 2 * i + 1;
        auto right_i = 2 * i + 2;
        // Ищем меньшего сына, если такой есть.
        auto lesser = i;
        if (left_i < used && less(array[left_i], array[lesser]))
            lesser = left_i;
        if (right_i < used && less(array[right_i], array[lesser]))
            lesser = right_i;
        // Если меньший сын есть, то проталкиваем корень в него.
        if (lesser != i) {
            std::swap(array[i], array[lesser]);
            i = lesser;
        } else {
            break;
        }
    }
    return array[used];
};

// алгоритм на куче:
// вводим данные в кучу
// пока в куче > 1 значения
//     вытаскиваем pop() 2 самых маленьких значения
//     суммируем, прибавляем сумму к общему_времени
//     запихиваем получившееся значение в кучу
// когда закончатся значения -
// возвращаем общее время.


int main() {

    PriorityQueue<int> heap;
    unsigned int input_len = 0;
    std::cin >> input_len;
    for (int i = 0; i < input_len; ++i) {
        std::cin >> heap;
    }
    int total_sum = 0;
    while (heap.size() > 1) {
        int sum = heap.pop() + heap.pop();
        total_sum += sum;
        heap.push(sum);
    }
    std::cout << total_sum;
    return EXIT_SUCCESS;
}
