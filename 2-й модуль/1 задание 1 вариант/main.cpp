// https://contest.yandex.ru/contest/7930/problems/1/
/* 1. Хеш-таблица
 * Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми.
 * Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества
 * и проверки принадлежности данной строки множеству.
 * Для разрешения коллизий используйте квадратичное пробирование.
 * i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
 *
 * Формат ввода
 * Каждая строка входных данных задает одну операцию над множеством.
 * Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
 * Тип операции – один из трех символов:
 *     + означает добавление данной строки в множество;
 *     - означает удаление строки из множества;
 *     ? означает проверку принадлежности данной строки множеству.
 * При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве.
 * При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.
 *
 * Формат вывода
 * Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того,
 * встречается ли данное слово в нашем множестве.
 * Пример
 * Ввод

+ hello
+ bye
? bye
+ bye
- bye
? bye
? hello

 * Вывод

OK
OK
OK
FAIL
OK
FAIL
OK

 */

#include <string>
#include <cstring>
#include <iostream>
#include <unordered_set>
#include <exception>


#define DELLPTR (reinterpret_cast<std::string*>(1))

// Хеш-функция строки.
// принимает указатель на нультерминированную строку
// и возвращает значение, 0 <= hash < max_has
namespace my {

    // алгоритм Горнера для вычисления хеша
    unsigned int hash(const char *str, const int max_hash) {
        const unsigned int a = 2654435769;
        unsigned int hash = 0;
        for (; *str != 0; ++str)
            hash = (hash * a + *str) % max_hash;
        return hash;
    }

    // перегрузка для std::string для удобства.
    unsigned int hash(const std::string &str, const int max_hash) {
        return hash(str.c_str(), max_hash);
    }

    // Квадратичное пробирование.
    // Возвращает последовательно индексы следующего элемента.
    // при _length степень двойки - выдаёт все индексы только один раз
    // конструктор принимает первый индек(это хеш строки) и длинну массива
    // https://en.wikipedia.org/wiki/Quadratic_probing
    class quadratic_probing {
    private:
        const unsigned int _length;
        unsigned int _i;
        unsigned int _index;
    public:
        quadratic_probing(unsigned int first, unsigned int length) : _length(length) {
            _i = 0;
            _index = first;
        };

        unsigned int next() {
            _index = (_index + _i) % _length;
            ++_i;
            return _index;
        };
    };

// реализуем класс hash
    class unordered_set {
    private:
        const unsigned int initial_size = 8;

        // текущая длинна массива
        unsigned int _length;
        // количество хранимых элементов.
        // Надо увеличивать размер, когда коэффициент заполнения таблицы достигает 3/4.
        unsigned int _capacity;
        // массив хранимых значений
        // (void*)0, если это пустая ячейка.
        // (void*)1, если это удалённая ячейка. Это также невалидный указатель, так как 4 кб защищается в начале программы минимум.
        std::string **_array;

        // увеличение в 4 раза. важно, что бы оставалось степенью двойки.
        void resize();


    public:
        unordered_set();

        ~unordered_set();

        bool add(const std::string &str);

        bool erase(const std::string &str);

        bool contain(const std::string &str) const;

        void debag_print() const;
    };

    unordered_set::unordered_set() {
        _array = new std::string *[initial_size];
        std::memset(_array, 0, sizeof(std::string *) * initial_size);
        _capacity = 0;
        _length = initial_size;
    };

    unordered_set::~unordered_set() {
        for (unsigned int i = 0; i < _length; ++i) {
            if (_array[i] != nullptr && _array[i] != DELLPTR) {
                delete _array[i];
            }
        }
        delete this->_array;
    };

    // алгоритм добавления
    // если надо - увеличиваем размер
    // заводим последовательность для поиска
    // и проходим по индексам, которые возвращает последовательность
    // если мы обратились, и валидный указатель и строки совпадают
    //     то возвращаем false
    // если мы обратились и удалено
    //     запоминаем это место
    //     идём дальше
    // если мы обратились и там пусто
    //     если было удалённое значение
    //         то записываем в него
    //     иначе записываем в найденное пустое
    // если вышли за пределы возвращаем false
    //     (такого быть не должно)
    bool unordered_set::add(const std::string &str) {
        bool two_thirds_full = (_length - _capacity * 2) < _capacity;
        if (two_thirds_full) {
            resize();
        }
        // последовательность индексов, в которые будем пытаться добавлять новое значение
        quadratic_probing qp_generator = quadratic_probing(hash(str, _length), _length);
        unsigned int first_deleted = 0;
        bool deleted_was = false;
        for (unsigned int i = 0; i < _length; i++) {
            unsigned int index = qp_generator.next();
            if (_array[index] == nullptr) { // если нашли пустое
                if (deleted_was) {
                    _array[first_deleted] = new std::string(str);
                } else {
                    _array[index] = new std::string(str);
                }
                ++_capacity;
                return true;
            } else if (_array[index] == DELLPTR) { // если нашли удалённое
                if (!deleted_was) {
                    deleted_was = true;
                    first_deleted = index;
                }
            } else if (*_array[index] == str) { // если нашли вставляемое значение
                return false;
            }
            // если нашли неравную строку, то просто идём дальше.
        }
        // попадаем сюда, если в map нет nullptr значений, только удалённые и не равные
        if (deleted_was) {
            _array[first_deleted] = new std::string(str);
            ++_capacity;
            return true;
        }
        throw std::overflow_error("Resize() was incorrect, no dell or null pointers, adding is not possible.");
    };


    // получаем последовательность значений
    // ищем для каждого, пока не встретим пустое значение
    bool unordered_set::erase(const std::string &str) {
        // последовательность индексов, в которых будем искать значения
        quadratic_probing qp_generator = quadratic_probing(hash(str, this->_length), this->_length);
        for (unsigned int i = 0; i < _length; i++) {
            unsigned int index = qp_generator.next();
            if (_array[index] == nullptr) {
                return false;
            } else if (_array[index] == DELLPTR) {
                // continue
            } else if (*_array[index] == str) {
                delete _array[index];
                _array[index] = DELLPTR;
                --_capacity;
                return true;
            }
        }
        return false;
    };

    // алгоритм:
    // получаем последовательность, по которой идём
    // если натыкаемся на не равную строку, идём вперёд
    //                 на удалённое значение идём вперёд
    //                 на конец, возвращаем  false
    //                 на равную строку, возвращаем true
    bool unordered_set::contain(const std::string &str) const {
        // последовательность индексов, в которых будем проверять значения
        quadratic_probing qp_generator = quadratic_probing(hash(str, _length), _length);
        for (unsigned int i = 0; i < _length; i++) {
            unsigned int index = qp_generator.next();
            if (_array[index] == nullptr) {
                return false;
            } else if (_array[index] == DELLPTR) {
                // continue
            } else if (*_array[index] == str) {
                return true;
            }
        }
        return false;
    };

    void unordered_set::resize() {
        unsigned int _new_length = _length * 4;
        std::string **_array_tmp = new std::string *[_new_length];
        std::memset(_array_tmp, 0, sizeof(std::string *) * _new_length);
        for (unsigned int i = 0; i < _length; ++i) {
            if (_array[i] != nullptr && _array[i] != DELLPTR) {
                // для каждого не пустого и не удалённого элемента массива
                // создаём последовательность индексов, в которые будем добавлять значение
                quadratic_probing qp_generator = quadratic_probing(hash(*_array[i], _new_length), _new_length);
                for (unsigned int j = 0; j < _new_length; j++) {
                    unsigned int index = qp_generator.next();
                    if (_array_tmp[index] == nullptr) { // если нашли пустое
                        _array_tmp[index] = _array[i];
                        break;
                    }
                }
            }
        }
        delete _array;
        _length = _new_length;
        _array = _array_tmp;
    }

    void unordered_set::debag_print() const {
        for (unsigned int i = 0; i < _length; i++) {
            if (this->_array[i] == nullptr) { // если нет значения
                std::cout << i << ": " << "␀\n";
            } else if (this->_array[i] == DELLPTR) {
                std::cout << i << ": " << "␡\n";
            } else {
                std::cout << i << ": " << *this->_array[i] << "\n";
            }
        }
    };

} // end namespase ::my

int main() {
    my::unordered_set string_set;

    std::string command;
    std::string word;
    while (true) {
        std::cin >> command >> word;
        if (!std::cin) { // если дескриптор закрыт.
            break;
        } else if (command == "+") {
            if (string_set.add(word)) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        } else if (command == "-") {
            if (string_set.erase(word)) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        } else if (command == "?") {
            if (string_set.contain(word)) {
                std::cout << "OK\n";
            } else {
                std::cout << "FAIL\n";
            }
        }
        // string_set.debag_print();
    }
    return 0;
}