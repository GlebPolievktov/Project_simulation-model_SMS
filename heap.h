#pragma once

#include <iostream>
#include <cmath>
using namespace std;

template<typename T> // Компаратор для стандартных типов данных
struct DefaulComparator {
    bool operator()(const T& l, const T& r) const {
        return l < r;
    }
};

// Шаблон кучи
template<typename T, typename Comparator = DefaulComparator<T>>
class Heap {
private:
    void sift_up(); // Просеивание
    void sift_down(); 
    void sizeAdd(); 
    size_t size; // Максимальная длина, при заполнении расширяется
    size_t count; // Количество элементов
    T* buffer; // Элементы в куче

    Comparator comp; // Метод сортировки кучи minHeap или maxHeap
public:
    Heap();
    Heap(const Heap&)=delete;
    Heap(const Heap&&)=delete;

    Heap& operator=(const Heap&)=delete;
    Heap& operator=(Heap&&)=delete;

    ~Heap();

    const T& top() const; // Обязательно const, чтобы не изменить значение
    void pop();
    void push(const T&);

    size_t GetCount() const;
    T* getBuffer() const;
};

// Конструктор по умолчанию, создаёт буфер размером 5
template<typename T, typename Comparator>
Heap<T, Comparator>::Heap(): size(5), count(0), buffer(new T[size]) {}

// Деструктор очищающий кучу
template<typename T, typename Comparator>
Heap<T, Comparator>::~Heap() {
    delete[] buffer;
}

// Удаление корня элемента
template<typename T, typename Comparator>
void Heap<T, Comparator>::pop() {
    if(count == 0) {
        return;
    }
    swap(buffer[0], buffer[count-1]);
    count--;
    sift_down();
}

// Восстанавливает свойство кучи при удалении элемента
template<typename T, typename Comparator>
void Heap<T, Comparator>::sift_down() {
    int current = 0; // Текущий элемент
    while(current * 2 + 1 < count) {
        int left = current * 2 + 1; // Левый потомок
        int right = current * 2 + 2; // Правый потомок
        // Определяем минимального из двух потомков
        int next = left;
        if(right < count && comp(buffer[right], buffer[left])) // КОМПАРАТОР
            next = right;
        // swap если минимальный элемент потомка, меньше текущего
        if(comp(buffer[next], buffer[current])) { // КОМПАРАТОР
            swap(buffer[next], buffer[current]);
            current = next; 
        }
        else break;
    }
}

// Добавляет в кучу элемент
template<typename T, typename Comparator>
void Heap<T, Comparator>::push(const T& value) {
    if(count == size) {
        sizeAdd(); // куча переполнена, нужно расширить
    }
    buffer[count++] = value;
    sift_up();
}

// Восстанавливает свойство кучи при добавлении элемента
template<typename T, typename Comparator>
void Heap<T, Comparator>::sift_up() {
    int i = count - 1; // текущий элемент
    while(comp(buffer[i], buffer[(i - 1) / 2]) && i > 0) { // Если родитель меньше текущего элемента, то swap 
        int j = (i - 1) / 2; // Родитель текущего элемента
        swap(buffer[i], buffer[j]);
        i = j;
    }    
}

// Расширение динамического массива
template<typename T, typename Comparator>
void Heap<T, Comparator>::sizeAdd() {
    size = size * 2;
    T* tempBuffer = new T[size];
    copy(buffer, buffer + count, tempBuffer);
    delete[] buffer;
    buffer = tempBuffer;
}

// Возвращаем корень кучи
template<typename T, typename Comparator>
const T& Heap<T, Comparator>::top() const {
    return buffer[0];
}

// Текущее количество элементов в куче
template<typename T, typename Comparator>
size_t Heap<T, Comparator>::GetCount() const {
    return count;
}

template <typename T, typename Comparator>
T* Heap<T, Comparator>::getBuffer() const {
    return buffer;
}
