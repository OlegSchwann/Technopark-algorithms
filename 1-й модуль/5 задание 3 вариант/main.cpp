// https://contest.yandex.ru/contest/7595/problems/5_3/
// Закраска прямой
// стандартный ввод, стандартный вывод
// На числовой прямой окрасили N отрезков.
// Известны координаты левого и правого концов каждого отрезка [Li, Ri].
// Найти длину окрашенной части числовой прямой.
// N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 109].
// Необходимо реализовать и использовать сортировку слиянием.
// Общее время работы алгоритма O(n log n).
// Формат ввода
//     В первой строке записано количество отрезков.
//     В каждой последующей строке через пробел записаны
//     координаты левого и правого концов отрезка.
// Формат вывода
//     Выведите целое число — длину окрашенной части.
// Пример
// Ввод
// 3         1 2 3 4 5 6 7 8
// 1 4       ┝━━━━━┥
// 7 8                   ┝━┥
// 2 5         ┝━━━━━┥
// Вывод
// 5         ┝━━━━━━━┥   ┝━┥ Считается расстояние "между"

#include <iostream>
#include <cstring>

// отрезок. Сортируем по началу массива
struct LineSegment {
    int begin;
    int end;

    int key() const {
        return begin;
    }

    bool operator<(const LineSegment &other) {
        //TODO
    }
};

// процедура слияния двуx массивов
// для структуры с любым сравнимым значением stored_value.key()
// оба входящих и результирующий отсортированы по возрастанию.
// в переданный третий массив
// first   len следующий индескс
// ↓       ↓
// 0 1 2 3



template<typename stored_value>
void merge(stored_value *const first_array, int first_len,
           stored_value *const second_array, int second_len,
           stored_value *result) {
    int first_i = 0, second_i = 0, result_i = 0;
    while (first_i < first_len && second_i < second_len) {
        if (first_array[first_i].key() < second_array[second_i].key()) {
            result[result_i] = first_array[first_i];
            ++first_i;
        } else {
            result[result_i] = second_array[second_i];
            ++second_i;
        }
        ++result_i;
    }
    // и теперь копируем остатки то ли в первом, то ли во втором
    while (first_i < first_len) {
        result[result_i] = first_array[first_i];
        ++first_i;
        ++result_i;
    }
    while (second_i < second_len) {
        result[result_i] = second_array[second_i];
        ++second_i;
        ++result_i;
    }
};

// процедура циклического вызова функции слияния
// каждый раз меняет массив из которого в который мы сливаем.
// source_array                 source_len следующий индескс
// ↓                            ↓
// □  □  □  □  □  □  □  □  □  □
//  □□    □□    □□    □□    □□
//    □□□□        □□□□      □□
//        □□□□□□□□          □□
//                □□□□□□□□□□
// пример
// sorted_len = 2 увеличивается в 2 раза во внешнем цикле
// ↓↓    ↓↓
// □□    □□    □□    □□    □□
// ↑           ↑           ↑
// i           i           i    // перемещается шагами по 2*sorted_len во внутреннем цикле.
template<typename stored_value>
void merge_sort(stored_value *source_array, int source_len) {
    // выделяем дополнительную память, размером с поступивший массив
    stored_value additional_array[source_len]; // todo: use new
    // флаг - если true то сливаем из источника в дополнительную память,
    // иначе из дополнительной памяти в источник.
    bool last_fused_in_source = true;
    // идём, увеличивая длинну отсортированных кчастков в 2 раза каждый шаг.
    for (int sorted_len = 1; sorted_len < source_len; sorted_len *= 2) {
        for (int i = 0; i < source_len; i += sorted_len * 2) {
            int first_len = 0, second_len = 0;
            if (i + sorted_len <= source_len) {
                first_len = sorted_len;
                if (i + sorted_len * 2 <= source_len) {
                    second_len = sorted_len;
                } else {
                    second_len = source_len - i - sorted_len;
                }
            } else {
                first_len = source_len - i;
                second_len = 0;
            }
            if (last_fused_in_source) {
                stored_value *first_array = source_array + i;
                stored_value *second_array = source_array + i + sorted_len;
                stored_value *result_array = additional_array + i;
                merge<stored_value>(first_array, first_len,
                                    second_array, second_len, result_array);

            } else {
                stored_value *first_array = additional_array + i;
                stored_value *second_array = additional_array + i + sorted_len;
                stored_value *result_array = source_array + i;
                merge<stored_value>(first_array, first_len,
                                    second_array, second_len, result_array);
            }
        }
        last_fused_in_source = !last_fused_in_source;
    }
    if (last_fused_in_source) {
        // то слили в результат, его и возвращаем
    } else {
        memcpy(source_array, additional_array, source_len * sizeof(stored_value));
    }
}


// функция, считающая суммарную длинну закрашенной области.
// отрезки должны быть отсортированы по возрастанию
// сливает тип с определёнными segment.begin, segment.end
// ┝━━━━━━━┥ → ┝━━━━━━━┥         ┝━━━━━━━┥    → ┝━━━━━━━━━━┥         ┝━━━━━━━┥      → ┝━┥, result += len(┝━━━━━━━┥)
//   ┝━┥                               ┝━━━━┥                                   ┝━┥
template<typename segment>
int connection_of_segments(segment *const source_array, int source_len) {
    if (source_len == 0) {
        return 0;
    }
    int result = 0;
    int begin = source_array[0].begin;
    int end = source_array[0].end;
    for (int i = 1; i < source_len; ++i) {
        if (source_array[i].begin <= end) {
            if (source_array[i].end <= end) {
                // просто следующую берём
            } else {
                end = source_array[i].end;
            }
        } else {
            result += end - begin;
            begin = source_array[i].begin;
            end = source_array[i].end;
        }
    }
    result += end - begin;
    return result;
}


int main() {
    int input_len = 0;
    std::cin >> input_len;
    LineSegment all_lines[input_len];
    for (int i = 0; i < input_len; ++i) {
        std::cin >> all_lines[i].begin >> all_lines[i].end;
    }
    merge_sort<LineSegment>(all_lines, input_len);
    int result = connection_of_segments<LineSegment>(all_lines, input_len);
    std::cout << result << std::endl;
    return 0;
}