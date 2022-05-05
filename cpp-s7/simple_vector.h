#pragma once

#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <iterator>

#include "array_ptr.h"

using namespace std;

class ReserveProxyObj {
public:
    ReserveProxyObj(const size_t rev_capacity)
        : capacity_(rev_capacity) {
    }

    size_t GetSize() const noexcept {
        return capacity_;
    }

private:
    size_t capacity_;

};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size)
        : items_(size) {
        if (size) {
            size_ = size;
            capacity_ = size;
            fill(items_.Get(), items_.Get() + size_, Type());
        }
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value)
        : items_(size)
        , size_(size)
        , capacity_(size) {
        fill(items_.Get(), items_.Get() + size_, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) 
        : items_(init.size())
        , size_(init.size())
        , capacity_(init.size()) {
        copy(init.begin(), init.end(), items_.Get());
    }

    SimpleVector(const SimpleVector& other)
        : items_(other.size_)
        , size_(other.size_)
        , capacity_(other.size_) {
        copy(other.begin(), other.end(), items_.Get());
    }

    SimpleVector(SimpleVector&& other) noexcept
        : items_(move(other.items_))
        , size_(exchange(other.size_, 0))
        , capacity_(exchange(other.capacity_, 0)) {
    }

    SimpleVector(ReserveProxyObj reserve)
        : items_(reserve.GetSize())
        , capacity_(reserve.GetSize()) {
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return (size_ == 0 ? true : false);
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        assert(index <= capacity_);
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        assert(index <= capacity_);
        return items_[index];
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return Iterator(items_.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return Iterator(&items_[size_]);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return ConstIterator(items_.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return ConstIterator(&items_[size_]);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return begin();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return end();
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        assert(pos >= begin() && pos <= end());
        auto value_copy = value;
        return Insert(pos, move(value_copy));
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        assert(pos >= cbegin() && pos <= cend());

        auto no_const_pos = const_cast<Iterator>(pos);
        auto pos_element = distance(begin(), no_const_pos);

        if (size_ < capacity_) {
            copy_backward(make_move_iterator(no_const_pos), make_move_iterator(end()), &items_[(size_ + 1)]);
            items_[pos_element] = move(value);
        }
        else {
            auto new_capacity = max(size_t(1), capacity_ * 2);
            ArrayPtr<Type> ptr(new_capacity);
            copy(make_move_iterator(&items_[0]), make_move_iterator(&items_[pos_element]), &ptr[0]);
            copy_backward(make_move_iterator(no_const_pos), make_move_iterator(end()), &ptr[(size_ + 1)]);
            ptr[pos_element] = move(value);
            items_.swap(ptr);
            capacity_ = new_capacity;
        }

        ++size_;
        return Iterator{ &items_[pos_element] };
    }

    SimpleVector& operator=(const SimpleVector& rhs) {
        if (this != &rhs) {
            if (rhs.IsEmpty()) {
                Clear();
                return *this;
            }
            SimpleVector<Type> ptr(rhs.size_);
            copy(rhs.begin(), rhs.end(), ptr.begin());
            ptr.capacity_ = rhs.capacity_;
            swap(ptr);
        }
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs) noexcept {
        if (this != &rhs) {
            if (rhs.IsEmpty()) {
                Clear();
                return *this;
            }
            SimpleVector<Type> ptr(rhs.size_);
            copy(rhs.begin(), rhs.end(), ptr.begin());
            ptr.capacity_ = rhs.capacity_;
            swap(ptr);
        }
        return *this;
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw out_of_range("Overflow");
        }
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw out_of_range("Overflow");
        }
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size == size_) {
            return;
        }

        if (new_size < size_) {
            for (auto it = &items_[new_size]; it != &items_[size_]; ++it) {
                *(it) = move(Type{});
            }
        }

        if (new_size > capacity_) {
            auto new_capacity = max(new_size, capacity_ * 2);
            ArrayPtr<Type> ptr(new_capacity);
            copy(make_move_iterator (&items_[0]), make_move_iterator(&items_[size_]), &ptr[0]);
            for (auto it = &ptr[size_]; it != &ptr[new_size]; ++it) {
                *(it) = move(Type{});
            }
            items_.swap(ptr);
            capacity_ = new_capacity;
        }

        size_ = new_size;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        auto item_copy = item;
        PushBack(move(item_copy));
    }

    void PushBack(Type&& item) {
        if (size_ < capacity_) {
            items_[size_] = move(item);
        }
        else {
            auto new_capacity = max(size_t(1), 2 * capacity_);
            ArrayPtr<Type> ptr(new_capacity);
            copy(make_move_iterator(&items_[0]), make_move_iterator(&items_[size_]), &ptr[0]);
            ptr[size_] = move(item);
            items_.swap(ptr);
            capacity_ = new_capacity;
        }
        ++size_;
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        assert(!IsEmpty());
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        assert(pos >= begin() && pos < end());
        auto no_const_pos = const_cast<Iterator>(pos);
        auto pos_element = distance(begin(), no_const_pos);
        move(++no_const_pos, end(), &items_[pos_element]);
        --size_;
        return &items_[pos_element];
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        items_.swap(other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            SimpleVector<Type> tmp_items(new_capacity);
            copy(cbegin(), cend(), tmp_items.begin());
            tmp_items.size_ = size_;
            swap(tmp_items);
        }
    }


private:
    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(rhs > lhs);
}