// https://contest.yandex.ru/contest/7595/problems/2_3/
// Пересечение множеств
// стандартный ввод, стандартный вывод
// Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию.
// A[0..n-1] и B[0..m-1]. n > m. Найдите их пересечение.
// Требуемое время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A.
// В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом
// поиска элемента B[i-1]. n, k ≤ 10000.
// Ввод        Вывод
// 5           1 3 5
// 3
// 1 2 3 4 5
// 1 3 5

// суть алгоритма, эффективно запускающего бинарный поиск
// идём по меньшему массиву B за O(m)
// заводим индекс начала поиска в A очередного элемента B[i]
// сначала = 0
// заводим индекс конца поиска в A очередного элемента B[i]
// сначала = 1
// в цикле
//     если A[от конца поиска] < текущего значения B[i]
//         то это значение может быть между
//             текущим значением конца и значением конца поиска в 2 раза меньше.
//             увеличиваем отрезок,
//             [0--1] 2  3  4  5  6  7  8  9  10  11  12  13  14  15  16
//              0 [1--2--3--4] 5  6  7  8  9  10  11  12  13  14  15  16
//              0  1  2  3 [4--5--6--7--8--9--10--11] 12  13  14  15  16
//              0  1  2  3  4  5  6  7  8  9  10 [11--12--13--14--15--16]
//             причём если после умножения индекс последнего выходит за пределы массива A, то
//                 мы округляем до последнего
//             проще всего сделать бинарный поиск на всём интервале, включая концы, чтоб не обрабатывать A[0] отдельно
// после того, как мы перешагнули через искомое значение в упорядоченном по возрастанию массиве
// (то есть A[начала поиска] < B[i] || B[i] <= A[конца поиска])
//     запускаем бинарный поиск от массива,
// он возвращает индекс найденного элемента или -1
// если -1 то просто берём следующее значение B
// если индекс найден -
//    добавляем в результирующий массив
//    и нижней границе поиска присваеваем найденный индекс, так как следующее значение B[i] точно больше, чем найденное.

#include <iostream>

// динамический массив, чтоб вернуть ответ пересечения массивов
// stored_type *array
// ↓
// 0 1 2 3 4 5 6 7 8 9
// + + + + + - - - - -
//           ↑         ↑
//           used      size
template<typename stored_type>
class Slice {
public:
    stored_type *array;
    int used;
    int size;

    Slice();

    Slice(const Slice &);

    ~Slice();

    void append(stored_type element);
};

template<typename stored_type>
Slice<stored_type>::Slice() {
    const int initial_size = 32;
    array = (stored_type *) malloc(initial_size * sizeof(stored_type));
    if (array == nullptr) {
        throw std::bad_alloc();
    }
    used = 0;
    size = initial_size;
}

template<typename stored_type>
Slice<stored_type>::Slice(const Slice &that) {
    array = (stored_type *) malloc(that.used * sizeof(stored_type));
    if (array == nullptr) {
        throw std::bad_alloc();
    }
    for (int i = 0; i < that.used; ++i) {
        this->array[i] = that.array[i];
    }
    this->used = that.used;
    this->used = that.used;
};

template<typename stored_type>
void Slice<stored_type>::append(stored_type element) {
    if (used == size) {
        int required_size = size * 2;
        auto new_array = (stored_type *) realloc(array, required_size * sizeof(stored_type));
        if (new_array == nullptr) {
            throw std::bad_alloc();
        } // не портим slice, если не можем выделить память
        array = new_array;
        size = required_size;
    }
    array[used] = element;
    ++used;
}

template<typename stored_type>
Slice<stored_type>::~Slice() {
    free(array);
}

// бинарный поиск(массив, индекс начала, индекс конца, искомое значение)
// ищет в отрезке за O(log())
// возвращает индекс найденного между началом и концом включая концы или SearchResult.is_found = false если не нашёл
// массив, первый, следующий за последним, искомое
int binary_search(const int * const array, int first_index, int after_last_index, const int searched_value){
    if ( first_index == after_last_index) {
        // массив пуст
        return -1;
    } else if (searched_value < array[first_index]) { // искомый элемент меньше всех в массиве
        return -1;
    } else if (array[after_last_index - 1] < searched_value) { // искомый элемент больше всех в массиве
        return -1;
    }
    // Если просматриваемый участок непустой, first_index < after_last_index
    while (first_index < after_last_index) {
        int center = first_index + (after_last_index - first_index) / 2;
        if (searched_value <= array[center])
            after_last_index = center;
        else
            first_index = center + 1;
    }
    if (array[after_last_index] == searched_value) {
        // Искомый элемент найден.
        return after_last_index;
    } else {
        return -1;
    }
}

Slice<int> intersection_search(
        const Slice<int> &A,
        const Slice<int> &B
) {
    Slice<int> result;
    int left_bound = 0;
    int after_right_bound = 1;
    for(int i = 0; i < B.used; ++i){
        while (A.array[after_right_bound-1] < B.array[i] && after_right_bound < A.used){
            after_right_bound = (after_right_bound * 2 > A.used)? A.used: after_right_bound * 2;
        }
        int found_index = binary_search(A.array, left_bound, after_right_bound, B.array[i]);
        if(found_index != -1){
            left_bound = found_index; // следующее будет точно не меньше.
            result.append(A.array[found_index]);
        }
    }
    return result;
};


int main() {
    Slice <int>A;
    Slice <int>B;
    int A_len = 0;
    std::cin >> A_len;
    int B_len = 0;
    std::cin >> B_len;
    int tmp = 0;
    for(int i = 0; i < A_len; ++i){
        std::cin >> tmp;
        A.append(tmp);
    }
    for(int i = 0; i < B_len; ++i){
        std::cin >> tmp;
        B.append(tmp);
    }
    auto result = intersection_search(A, B);

    for(int i = 0; i < result.used; ++i){
        std::cout << result.array[i];
        if(i != result.used - 1){
            std::cout << " ";
        } else {
            std::cout << std::endl;
        }
    }
    return 0;
}