// https://contest.yandex.ru/contest/7595/problems/6/
// Порядковая статистика
// стандартный ввод, стандартный вывод
// Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..109] размера n.
// Требуется найти k-ю порядковую статистику - напечатать число,
// которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
// Напишите нерекурсивный алгоритм.
// Требования к дополнительной памяти: O(n).
// Требуемое среднее время работы: O(n).
// Функцию Partition следует реализовывать методом прохода двумя итераторами от конца массива к началу.
// Реализуйте стратегию выбора опорного элемента "случайный элемент".
//
// // Описание для случая прохода от начала массива к концу:
// Выбирается опорный элемент как элемент по случайному индексу.
// Опорный элемент меняется с последним элементом массива.
// Во время работы Partition в начале массива содержатся элементы, не бо́льшие опорного.
// Затем располагаются элементы, строго бОльшие опорного.
// В конце массива лежат нерассмотренные элементы.
// Последним элементом лежит опорный.
// Итератор (индекс) i указывает на начало группы элементов, строго бо́льших опорного.
// Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
// Шаг алгоритма, пока j < a.len:
//     Рассматривается элемент, на который указывает j.
//     Если он больше опорного,
//         то сдвигаем j.
//     Если он не больше опорного,
//         то меняем a[i] и a[j] местами,
//         сдвигаем i и сдвигаем j.
// В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.

//  0123456789 <= опорного < 𝟬𝟭𝟮𝟯𝟰𝟱𝟲𝟳𝟴𝟵
//||2 8 7 1 3 5 6|4
// |2|8 7 1 3 5 6|4
//  2|𝟴|7 1 3 5 6|4
//  2|𝟴 𝟳|1 3 5 6|4
//  2 1|𝟳 𝟴|3 5 6|4
//  2 1 3|𝟴 𝟳|5 6|4
//  2 1 3|𝟴 𝟳 𝟱|6|4
//  2 1 3|𝟴 𝟳 𝟱 𝟲|4
//  2 1 3 4|𝟳 𝟱 𝟲 𝟴|

// Ввод
// 10 4
// 1 2 3 4 5 6 7 8 9 10
// Вывод
// 5

// Ввод
// 10 0
// 3 6 5 7 2 9 8 10 4 1
// Вывод
// 1

// Ввод
// 10 9
// 0 0 0 0 0 0 0 0 0 1
// Вывод
// 1

#include <iostream>
#include <random>
#include <vector>

// возвращает элементы между left_index и right_index включая концы.
// если left_index == right_index то возвращает его, нельзя допустить деления на 0.
int get_pivot_index(int left_index, int right_index) {
    if(left_index == right_index){
        return left_index;
    }
    int pivot_index = left_index + rand() % (right_index - left_index);
    return pivot_index;
}

// прямой проход итераторов
template<class stored_type>
int Partition(stored_type *array, int left_index, int right_index) {
    int pivot_index = get_pivot_index(left_index, right_index);
    std::swap(array[pivot_index], array[right_index]); // ставим опорный в конец
    stored_type pivot = array[right_index];
    int last_not_greater = left_index - 1;
    for (auto first_not_considered = left_index; first_not_considered < right_index; ++first_not_considered) {
        if (array[first_not_considered] < pivot) {
            ++last_not_greater;
            std::swap(array[last_not_greater], array[first_not_considered]);
        }
    }
    std::swap(array[last_not_greater + 1], array[right_index]);
    return last_not_greater + 1;
}

// обратный проход итераторов
template<class stored_type>
int Partition2(stored_type *array, int left_index, int right_index) {
    int pivot_index = get_pivot_index(left_index, right_index);
    std::swap(array[pivot_index], array[left_index]);
    stored_type pivot = array[left_index];
    int first_greater = right_index + 1;
    for (auto first_not_considered = right_index; first_not_considered > left_index; --first_not_considered) {
        if (array[first_not_considered] > pivot) {
            --first_greater;
            std::swap(array[first_greater], array[first_not_considered]);
        }
    }
    std::swap(array[first_greater - 1], array[left_index]);
    return first_greater - 1;
}

template<class stored_type>
stored_type ordinal_statistics(stored_type *array, int end_index, int searched_index) {
    int begin_index = 0;
    while (true) {
        int founded_index = Partition2(array, begin_index, end_index);
        if (founded_index - begin_index == searched_index) {
            return array[founded_index];
        } else if (founded_index - begin_index + 1 > searched_index) {
            end_index = founded_index - 1;
        } else {
            searched_index = searched_index - (founded_index - begin_index + 1);
            begin_index = founded_index + 1;
        }
    }
};

int main() {
    size_t array_len = 0, reqired_index = 0;
    std::cin >> array_len >> reqired_index;
    int vector[array_len];
    for (int i = 0; i < array_len; i++) {
        int tmp = 0;
        std::cin >> tmp;
        vector[i] = tmp;
    }
    int found_value = ordinal_statistics(vector, array_len - 1, reqired_index);
    std::cout << found_value << std::endl;
    return 0;
}
