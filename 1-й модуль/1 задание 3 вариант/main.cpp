// https://contest.yandex.ru/contest/7595/problems/1_3/
// Нужная сумма
// стандартный ввод, стандартный вывод
// Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
// Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
// Время работы O(n + m). n, m ≤ 100000.
// Идея.
// a_index = начало A
// b_index = конец B
// в цикле:
// если: A[a_index] + B[b_index]
//     больше k → уменьшаем сумму, уменьшая B[b_index] →
//         --b_index;
//     равно k  → увеличиваем счётчик пар →
//         ++a_index; --b_index;
//     меньше k → увеличиваем сумму, увеличивая A[a_index] →
//         ++a_index;
//     если какой-то из индексов дошёл до конца →
//         то не будет больше сумм = k, выходим из цикла.
//
// Ввод
// 4
// -5 0 3 18
// 5
// -10 -2 4 7 12
// 7
// Вывод
// 3

#include <iostream>

int number_of_pairs(int *A, u_int A_len, int *B, u_int B_len, int k){
    u_int required_pairs = 0;
    for(int a_index = 0, b_index = B_len - 1;
        a_index < A_len && b_index > -1; ){
        int sum = A[a_index] + B[b_index];
        if(sum > k){
            --b_index;
        } else if (sum < k) {
            ++a_index;
        } else { // sum == k
            ++required_pairs;
            ++a_index;
            --b_index;
        }
    }
    return required_pairs;
}


int main() {
    // A массив
    u_int A_len = 0;
    std::cin >> A_len;
    int A[A_len];
    for(int i = 0; i < A_len; ++i){
        std::cin >> A[i];
    }
    // B_массив
    u_int B_len = 0;
    std::cin >> B_len;
    int B[B_len];
    for(int i = 0; i < B_len; ++i){
        std::cin >> B[i];
    }
    // k - нужная сумма
    int k = 0;
    std::cin >> k;
    // ответ - количество нужных сумм.
    int result = number_of_pairs(A, A_len, B, B_len, k);
    std::cout << result << std::endl;
    return 0;
}
