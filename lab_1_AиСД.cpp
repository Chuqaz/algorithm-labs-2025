#include <iostream>
#include <random>
#include <utility>
#include <cmath>
#include <stdexcept>

using namespace std;


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
    ~Class() {
        delete[] plenty;
    }
};