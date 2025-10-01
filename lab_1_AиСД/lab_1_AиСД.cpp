#include <iostream>
#include <random>
#include <utility>
#include <cmath>
#include <stdexcept>

using namespace std;

template<typename A, typename B>
ostream& operator<<(std::ostream& os, const std::pair<A, B>& p) {
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}


template<typename T>
class Class {
private:
    size_t _size;
    T* plenty;
public:
    static constexpr double e = 1e-6;
    Class() :plenty(nullptr), _size(0) {}
    Class(T* data, size_t size) :_size(size) {
        plenty = new T[_size];
        for (int i = 0; i < size; i++) {
            plenty[i] = data[i];
        }
    }
    Class(const Class<T>& other) {
        _size = other._size;
        plenty = new T[_size];
        for (int i = 0; i < _size; i++) {
            plenty[i] = other.plenty[i];
        }
    }
    Class& operator=(Class<T> other) {
        if (*this == other) return *this;
        delete[] plenty;
        _size = other._size;
        plenty = new T[_size];
        for (int i = 0; i < _size; i++) {
            plenty[i] = other.plenty[i];
        }
        return *this;
    }
    Class(T min, T max, size_t size) :_size(size) {
        random_device rd;
        mt19937 gen(rd());
        plenty = new T[size];
        for (int i = 0; i < _size; i++) {
            plenty[i] = 0;
        }
        if constexpr (is_integral_v<T>) {
            uniform_int_distribution<T> distrib(min, max);
            for (int i = 0; i < size; i++) {
                T random_number;
                do {
                    random_number = distrib(gen);
                } while (contains(plenty, i, random_number));
                plenty[i] = random_number;
            }
        }
        else if constexpr (is_floating_point_v<T>) {
            uniform_real_distribution<T> distrib(min, max);
            for (int i = 0; i < size; i++) {
                T random_number;
                do {
                    random_number = distrib(gen);
                } while (contains(plenty, i, random_number));
                    plenty[i] = random_number;
            }
        }
    }
    bool equal(T a, T b) {//Проверка на равенство
        if constexpr (is_floating_point_v<T>) {
            if (fabs(a - b) < e) {
                return true;
            }
            else
                return false;
        }
        else
            return a==b;
    }
    T operator[](int index) {
        if (index >= _size) throw out_of_range("Out of range");
        return plenty[index];
    }
    Class operator+(const Class<T> other) {//Сумма множеств
        size_t result_size = _size;
        for (int i = 0; i < other._size; i++) {
            if (!contains(plenty, _size, other.plenty[i])) {
                result_size++;
            }
        }
        Class result;
        result._size = result_size;
        result.plenty = new T[result_size];
        for (int i = 0; i < _size; i++) {
            result.plenty[i] = plenty[i];
        }

        size_t j = _size;
        for (int i = 0; i < other._size; i++) {
            if (!contains(plenty, _size, other.plenty[i]))
                if (j < result_size) {
                    result.plenty[j++] = other.plenty[i];
                }
        }
        return result;
    }
    Class operator-(const Class<T> other) {//Разность множеств
        int result_size = 0;
        for (int i = 0; i < _size; i++) {
            if (!contains(other.plenty, other._size, plenty[i])) {
                result_size++;
            }
        }
        Class result;
        result._size = result_size;
        result.plenty = new T[result_size];
        int j = 0;
        for (int i = 0; i < _size; i++) {
            if (!contains(other.plenty, other._size, plenty[i])) {
                if (j < result_size) {
                    result.plenty[j++] = plenty[i];
                }
            }
        }
        return result;
    }
    Class operator+(T number) {//Операторы прибавления числа к множеству
        if (contains(plenty, _size, number)) {
            return *this;
        }
        Class result;
        result.plenty = new T[_size + 1];
        result._size = _size + 1;
        for (int i = 0; i < _size; i++) {
            result.plenty[i] = plenty[i];
        }
        result.plenty[_size] = number;
        return result;
    }
    void operator+=(T number) {
        if (contains(plenty, _size, number)) {
            return;
        }
        T* new_plenty = new T[_size + 1];
        for (int i = 0; i < _size; i++) {
            new_plenty[i] = plenty[i];
        }
        new_plenty[_size] = number;
        delete[] plenty;
        plenty = new_plenty;
        _size++;
    }
    Class operator-(T number) {//Операторы вычитания числа из множества
        if (!contains(plenty, _size, number)) {
            return *this;
        }
        Class result;
        result.plenty = new T[_size - 1];
        result._size = _size - 1;
        int j = 0;
        for (int i = 0; i < _size; i++) {
            if (!equal(plenty[i],number)) {
                if (j < result._size) {
                    result.plenty[j++] = plenty[i];
                }
            }
        }
        return result;
    }
    void operator-=(T number) {
        if (!contains(plenty, _size, number)) {
            return;
        }
        T* new_plenty = new T[_size - 1];
        int j = 0;
        for (int i = 0; i < _size; i++) {
            if (!equal(plenty[i], number)) {
                new_plenty[j++] = plenty[i];
            }
        }
        delete[] plenty;
        plenty = new_plenty;
        _size--;
    }

    Class symmetricDifference(Class<T> other) {//Метод  реализации симметрической разнцы
        return (other - *this) + (*this - other);
    }

    Class intersection(Class<T> other) {// Метод для определения пересечения
        int result_size = 0;
        for (int i = 0; i < other._size; i++) {
            if (contains(plenty, _size, other.plenty[i])) {
                result_size++;
            }
        }
        Class result;
        result._size = result_size;
        result.plenty = new T[result_size];
        for (int i = 0; i < result_size; i++) {
            result.plenty[i] = T();
        }
        int j = 0;
        for (int i = 0; i < other._size; i++) {
            if (contains(plenty, _size, other.plenty[i])) {
                if (j < result_size) {
                    result.plenty[j++] = other.plenty[i];
                }
            }
        }
        return result;
    }
    bool operator==(Class<T> other) {
        if (_size != other._size) return false;
        for (int i = 0; i < _size; i++) {
            if (!other.contains(other.plenty, other._size, plenty[i])) return false;
        }
        return true;
    }
    bool operator!=(Class<T> other) { return  !(*this == other); }

    bool contains(T* data, size_t size, T value) {//Метод для проверки наличия числа
        for (int i = 0; i < size; i++) {
            if (equal(data[i],value)) {
                return true;
            }
        }
        return false;
    }
    bool containsElement(T number) {//Метод для проверки наличия числа
        if (contains(plenty,_size ,number)) {
            return true;
        }
        else
            return false;
    }
    friend ostream& operator<< <>(std::ostream& os, const Class<T> other);
    
    ~Class() {
        delete[] plenty;
    }
};

template<typename T>
ostream& operator<<(std::ostream& os, const Class<T> other) {
    for (int i = 0; i < other._size; i++) {
        os << other.plenty[i];
        if (i < other._size - 1) {
            os << ", ";
        }
    }
    os << '\n';
    return os;
}

int main()
{
    float arr2[] = { 3.1,1.5,4.4,-5,6,-7.1 };

    Class<float> a(1,10,6);
    Class<float> b(arr2, 6);

    cout << "Set A: ";
    cout<<a;
    cout << "Set B: ";
    cout << b;
    Class<float> d=a+b;
    cout << "Set D: ";
    cout << d;
    Class<float> e = a - b;
    cout << "Set E: ";
    cout << e;
    Class<float> e_1 = b - 3.4;
    cout << "Set E_1: ";
    cout << e_1;
    e_1 -= 4.6;
    cout << e_1;
    e_1 += 7.1;
    cout << e_1;
    Class<float> e_2 = a + 10;
    cout << "Set E_2: ";
    cout << e_2;
    Class<float> e_3 = a.intersection(b);
    cout << e_3;
    cout<<e_2.containsElement(2);
};
