#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <vector>

using namespace std;

template <typename Type>
class SingleLinkedList {
    // ���� ������
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value{};
        Node* next_node = nullptr;
    };

    // ������ ������ ������� ��������.
    // ���������� ��������� ��������� �� �������� ������������ ������
    // ValueType - ��������� � Type (��� Iterator) ���� � const Type (��� ConstIterator)
    template <typename ValueType>
    class BasicIterator {
        // ����� ������ ����������� �������������, ����� �� ������� ������
        // ��� ������ � ��������� ������� ���������
        friend class SingleLinkedList;

        // �������������� ����������� ��������� �� ��������� �� ���� ������
        explicit BasicIterator(Node* node)
            : node_(node) {
        }

    public:
        // ����������� ���� ���� �������� ����������� ���������� � ��������� ����� ���������

        // ��������� ��������� - forward iterator
        // (��������, ������� ������������ �������� ���������� � ������������ �������������)
        using iterator_category = std::forward_iterator_tag;
        // ��� ���������, �� ������� ������������ ��������
        using value_type = Type;
        // ���, ������������ ��� �������� �������� ����� �����������
        using difference_type = std::ptrdiff_t;
        // ��� ��������� �� ����������� ��������
        using pointer = ValueType*;
        // ��� ������ �� ����������� ��������
        using reference = ValueType&;

        BasicIterator() = default;

        // �������������� �����������/����������� �����������
        // ��� ValueType, ����������� � Type, ������ ���� ����������� ������������
        // ��� ValueType, ����������� � const Type, ������ ���� ��������������� ������������
        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_(other.node_) {
        }

        // ����� ���������� �� ������� �������������� �� ���������� ��������� = ��� �������
        // ����������������� ������������ �����������, ���� ������� �������� = �
        // �������� ���������� ������������� ��� �� ���.
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // �������� ��������� ���������� (� ���� ������� ��������� ��������� ����������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // ��������, �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        // �������� ��������� ���������� (� ���� ������� ��������� ��������)
        // ��� ��������� �����, ���� ��� ��������� �� ���� � ��� �� ������� ������, ���� �� end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // ��������, �������� ���������� �� �����������
        // �������������� !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return !(*this == rhs);
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������
        // ���������� ������ �� ������ ����
        // ��������� ���������, �� ������������ �� ������������ ������� ������, �������� � �������������� ���������
        BasicIterator& operator++() noexcept {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        // �������� ��������������. ����� ��� ������ �������� ��������� �� ��������� ������� ������.
        // ���������� ������� �������� ���������
        // ��������� ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        BasicIterator operator++(int) noexcept {
            auto this_copy(*this);
            ++(*this);
            return this_copy;
        }

        // �������� �������������. ���������� ������ �� ������� �������
        // ����� ����� ���������, � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
            return node_->value;
        }

        // �������� ������� � ����� ������. ���������� ��������� �� ������� ������� ������.
        // ����� ����� ���������, � ���������, �� ������������ �� ������������ ������� ������,
        // �������� � �������������� ���������
        [[nodiscard]] pointer operator->() const noexcept {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // ��������, ����������� ��������� ��������� ������
    using Iterator = BasicIterator<Type>;
    // ����������� ��������, ��������������� ������ ��� ������ � ��������� ������
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList() = default;

    SingleLinkedList(std::initializer_list<Type> values) {
        Assign(values.begin(), values.end());  // ����� ������� ����������
    }

    SingleLinkedList(const SingleLinkedList& other) {
        Assign(other.begin(), other.end());  // ����� ������� ����������
    }

    ~SingleLinkedList() {
        Clear();
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs) {
            Assign(rhs.begin(), rhs.end());
        }
        return *this;
    }

    // ���������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator{ head_.next_node };
    }

    // ���������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� end()
    // ��������� ������ ������������ ������ ������ cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return cbegin();
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    // ��������� ������ ������������ ������ ������ cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }

    // ���������� ����������� ��������, ����������� �� ������ �������
    // ���� ������ ������, ������������ �������� ����� ����� cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return ConstIterator{ head_.next_node };
    }

    // ���������� ����������� ��������, ����������� �� �������, ��������� �� ��������� ��������� ������������ ������
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }

    // ���������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] Iterator before_begin() noexcept {
        return Iterator{ &head_ };
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        return ConstIterator{ const_cast<Node*>(&head_) };
    }

    // ���������� ����������� ��������, ����������� �� ������� ����� ������ ��������� ������������ ������.
    // �������������� ���� �������� ������ - ������� ������������� ������� � �������������� ���������
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return cbefore_begin();
    }

    // ���������� ���������� ��������� � ������ �� ����� O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    // ��������, ������ �� ������ �� ����� O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return GetSize() == 0;
    }

    // ��������� ������� value � ������ ������ �� ����� O(1)
    void PushFront(const Type& value) {
        // ���� �������� new �������� ����������, ��������� head_.next_node ��������� �������
        head_.next_node = new Node(value, head_.next_node);

        // ����������� ��� �� ����������� ����������
        ++size_;
    }

    /*
     * ��������� ������� value ����� ��������, �� ������� ��������� pos.
     * ���������� �������� �� ����������� �������
     * ���� ��� �������� �������� ����� ��������� ����������, ������ ��������� � ������� ���������
     */
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        auto& prev_node = pos.node_;
        assert(prev_node);

        auto p_new_node = new Node(value, prev_node->next_node);
        p_new_node->next_node = prev_node->next_node;
        prev_node->next_node = p_new_node;

        // ���� �������� new �������� ����������, ��������� prev_node->next_node ��������� �������
        //prev_node->next_node = new Node(value, prev_node->next_node);

        // ����������� ��� �� ����������� ����������
        ++size_;
        return Iterator{ prev_node->next_node };
    }

    void PopFront() noexcept {
        assert(!IsEmpty());
        auto ptr = head_.next_node;
        head_.next_node = ptr->next_node;
        delete ptr;
        --size_;
    }

    /*
     * ������� �������, ��������� �� pos.
     * ���������� �������� �� �������, ��������� �� ��������
     */
    Iterator EraseAfter(ConstIterator pos) noexcept {
        assert(!IsEmpty());
        Node* const node_to_erase = pos.node_->next_node;
        pos.node_->next_node = node_to_erase->next_node;
        delete node_to_erase;
        --size_;
        return Iterator{ pos.node_->next_node };
    }

    // ������� ������ �� ����� O(N)
    void Clear() noexcept {
        // ������� �������� ������� � ������ ������, ���� ��� �� ����������
        while (head_.next_node != nullptr) {
            auto p_next = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = p_next;
        }
        size_ = 0;
    }

    // ���������� ���������� ������� �� ����� O(1)
    void swap(SingleLinkedList& other) noexcept {
        std::swap(head_.next_node, other.head_.next_node);
        std::swap(size_, other.size_);
    }

private:
    template <typename InputIterator>
    void Assign(InputIterator from, InputIterator to) {
        // ������ ��������� ������, � ������� ����� ��������� �������� �� ��������� [from, to)
        // ���� � �������� ���������� ����� ��������� ����������,
        // ��� ���������� ��������� ��� ������
        SingleLinkedList<Type> tmp;

        // �������� ����� ������������ ������� � ��������� �� ������ ����
        Node** node_ptr = &tmp.head_.next_node;
        while (from != to) {
            // ���������, ��� ������� ��������� - �������
            assert(*node_ptr == nullptr);

            // ������ ����� ���� � ���������� ��� ����� � ��������� �������� ����
            *node_ptr = new Node(*from, nullptr);
            ++tmp.size_;

            // ������ node_ptr ������ ����� ��������� �� ��������� ����
            node_ptr = &((*node_ptr)->next_node);

            // ��������� � ���������� �������� ���������
            ++from;
        }

        // ������, ����� tmp �������� ����� ��������� ��������� [from, to),
        // ����� ��������� ����� ������� �������� ������� � tmp
        swap(tmp);
        // ������ ������� ������ �������� ����� ��������� ��������� [from, to),
        // � tmp - ������� �������� �������� ������
    }

    // ��������� ����, ������������ ��� ������� "����� ������ ���������"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (&lhs == &rhs)  // ����������� ��������� ������ � �����
        || (lhs.GetSize() == rhs.GetSize()
            && std::equal(lhs.begin(), lhs.end(), rhs.begin()));  // ����� ������� ����������
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs == rhs);  // ����� ������� ����������
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());  // ����� ������� ����������
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);  // ����� ������� ����������
}

template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return (rhs < lhs);  // ����� ������� ����������
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(lhs < rhs);  // ����� ������� ����������
}


// ������� ���������� �������������� ������,
// ����������� ������� � ��������� ������
template <class Type>
auto MakeInsertingFunction(vector<SingleLinkedList<Type>>& lists, int x) {
    return [&, x](const Type& value) {
        lists[x].PushFront(value);
    };
}

template <class T>
void InsertRange(int from, int to, T push_function) {
    for (int i = from; i < to; ++i) {
        push_function(i);
    }
}

//
void TestErrors() {
    // main ��������� ������, � �� ��� ������.
    // �� ������� ��� ���� �������
    // ������ ���� ������, ������ ������ ��������� ��� ������� ����� ������.
    // ����� ������ � ���� ������ �
    // ������ �������� ����� �����

    vector<SingleLinkedList<int>> lists_a(10);

    auto push_to_2a = MakeInsertingFunction(lists_a, 2);
    auto push_to_5a = MakeInsertingFunction(lists_a, 5);
    auto push_to_7a = MakeInsertingFunction(lists_a, 7);

    InsertRange(10, 12, push_to_2a);
    InsertRange(12, 14, push_to_5a);
    InsertRange(14, 16, push_to_7a);

    assert(lists_a[2] == SingleLinkedList<int>({ 11, 10 }));
    assert(lists_a[5] == SingleLinkedList<int>({ 13, 12 }));
    assert(lists_a[7] == SingleLinkedList<int>({ 15, 14 }));

    vector<SingleLinkedList<int>> lists_b = lists_a;

    auto push_to_2b = MakeInsertingFunction(lists_b, 2);
    auto push_to_5b = MakeInsertingFunction(lists_b, 5);
    auto push_to_7b = MakeInsertingFunction(lists_b, 7);

    InsertRange(20, 22, push_to_2b);
    InsertRange(22, 24, push_to_5b);
    InsertRange(24, 26, push_to_7b);

    assert(lists_b[2] == SingleLinkedList<int>({ 21, 20, 11, 10 }));
    assert(lists_b[5] == SingleLinkedList<int>({ 23, 22, 13, 12 }));
    assert(lists_b[7] == SingleLinkedList<int>({ 25, 24, 15, 14 }));

    lists_a[2].PopFront();
    lists_a[5].InsertAfter(lists_a[5].begin(), 100);
    lists_b[5].EraseAfter(next(lists_b[5].begin()));
    lists_b[7].Clear();

    assert(lists_a[2] == SingleLinkedList<int>({ 10 }));
    assert(lists_a[5] == SingleLinkedList<int>({ 13, 100, 12 }));
    assert(lists_b[5] == SingleLinkedList<int>({ 23, 22, 12 }));
    assert(lists_b[7] == SingleLinkedList<int>());
}