// https://contest.yandex.ru/contest/7595/problems/7_2/
// LSD для long long
// стандартный ввод, стандартный вывод
// Дан массив неотрицательных целых 64-битных чисел.
// Количество чисел не больше 1000000.
// Отсортировать массив методом поразрядной сортировки LSD по байтам.
// Ввод
// 3
// 4 1000000 7
// Вывод
// 4 7 1000000

// заводим массив int[256] - cчётчик чисел для каждого значения разряда
// для каждого разряда 64-х битного числа
// проход первый - считаем количество.
// вычленяем разряд, обращаясь к счётчику по этому байту увеличиваем на 1
// подготовка
// заводим массив [256] указатель на начало массива,
// такой что все меньшие числа точно влезут, без зазора,
// и счётчик = 0.
// проход второй
// вычленяем разряд, кладём в соответствующий массив по его счётчику, увеличиваем счётчик
// теперь переключаемся на другой массив

// исходно: [1, 3, 5, 8, 2, 1, 7, 2, 6, 4, 0, 1, 4, 3, 5, 6]
//
// значение разряда[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
// количество      [1, 3, 2, 1, 2, 2, 2, 1, 1, 0]
//
// подготовленный второй массив
// 0 1   2  3 4  5  6  7 8 9
// □ □□□ □□ □ □□ □□ □□ □ □
//
// и распихиваем(не забываем о других разрядах)
// распихиваем идя слева на право, поэтому сортировка устойчива
// 0 111 22 3 44 55 66 7 8

#include <iostream>

struct Subarray {
    unsigned long long int *digit_array;
    unsigned int counter;
};

// первый вариант - побитовый и компактный
void byte_by_byte_sorting(unsigned long long int *source_array, unsigned int sorse_len) {
    unsigned long long int additional_array[sorse_len];
    unsigned long long int *from_array = source_array, *to_array = additional_array;
    for (unsigned long long int mask = 0b11111111, number_of_zeros = 0; mask != 0; mask <<= 8, number_of_zeros += 8) {
        unsigned int byte_value_counter[0b100000000] = {0};
        unsigned int total_offset = 0;
        Subarray arrays_for_each_digit[0b100000000] = {nullptr, 0};
        for (unsigned int i = 0; i < sorse_len; ++i) {
            ++byte_value_counter[(from_array[i] & mask) >> number_of_zeros];
        }
        for (unsigned int i = 0; i < 0b100000000; ++i) {
            arrays_for_each_digit[i].digit_array = &to_array[total_offset];
            total_offset += byte_value_counter[i];
        }
        for (unsigned int i = 0; i < sorse_len; ++i) {
            Subarray &current_digit = arrays_for_each_digit[(from_array[i] & mask) >> number_of_zeros];
            current_digit.digit_array[current_digit.counter++] = from_array[i];
        }
        unsigned long long int *tmp = from_array;
        from_array = to_array;
        to_array = tmp;
    }
}

// для сортировки любого примитивного беззнакового типа
// принимает исходный и результирующий массивы, их длинну, и номер байта, по которому всё сортируется.
// номер байта с младшего, начиная с нуля, номер < sizeof(int_type)
template<typename int_type>
void one_byte_sorting(int_type *const from_array, int_type *to_array,
                      unsigned int array_len, char number_byte_to_sort) {
    int_type const mask = (int_type)0b11111111 << (number_byte_to_sort*8);
    auto const number_of_zeros = (unsigned char) (8 * number_byte_to_sort);
    unsigned int byte_value_counter[0b100000000] = {0};
    unsigned int total_offset = 0;
    Subarray arrays_for_each_digit[0b100000000] = {nullptr, 0};
    for (unsigned int i = 0; i < array_len; ++i) {
        ++byte_value_counter[(from_array[i] & mask) >> number_of_zeros];
    }
    for (unsigned int i = 0; i < 0b100000000; ++i) {
        arrays_for_each_digit[i].digit_array = &to_array[total_offset];
        total_offset += byte_value_counter[i];
    }
    for (unsigned int i = 0; i < array_len; ++i) {
        Subarray &current_digit = arrays_for_each_digit[(from_array[i] & mask) >> number_of_zeros];
        current_digit.digit_array[current_digit.counter++] = from_array[i];
    }
}

// 2 вариант - обобщённый
template<typename int_type>
void byte_by_byte_sorting2(int_type *source_array, unsigned int sourse_len) {
    auto additional_array = new int_type[sourse_len];
    int_type *from_array  = source_array;
    int_type *to_array    = additional_array;
    for (unsigned char i = 0; i < sizeof(int_type); ++i) {
        one_byte_sorting(from_array, to_array, sourse_len, i);
        unsigned long long int *tmp = from_array;
        from_array = to_array;
        to_array = tmp;
    }
    delete[](additional_array);
}

int main() {
    unsigned int input_len;
    std::cin >> input_len;
    unsigned long long int input_array[input_len];
    for (unsigned int i = 0; i < input_len; ++i) {
        std::cin >> input_array[i];
    }
    byte_by_byte_sorting2<unsigned long long int>(input_array, input_len);
    for (unsigned int i = 0; i < input_len; ++i) {
        std::cout << input_array[i] << " ";
    }
    return 0;
}
/*
8
255 65280 16711680 4278190080 1095216660480 280375465082880 71776119061217280 18374686479671623680
 */