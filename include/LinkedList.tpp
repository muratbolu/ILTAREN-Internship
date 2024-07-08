#pragma once

#include "IObjectPool.tpp"
#include "Node.tpp"
#include "NodeIter.tpp"

#include <cstdio>

template<typename T>
class LinkedList
{
public:
    constexpr LinkedList() noexcept = default;

    constexpr LinkedList(IObjectPool<Node<T>>* pool) noexcept :
        mNodePool { pool }
    {
    }

    constexpr ~LinkedList() noexcept
    {
        deleteAll(mHead);
    }

    constexpr LinkedList(const LinkedList& obj) noexcept :
        mNodePool { obj.mNodePool }
    {
        for (const T& o : obj)
        {
            push_back(o);
        }
    }

    // Manual std::exchange
    constexpr LinkedList(LinkedList&& obj) noexcept :
        mHead { obj.mHead },
        mTail { obj.mTail },
        mNodePool { obj.mNodePool },
        mSize { obj.mSize }
    {
        obj.mHead = nullptr;
        obj.mTail = nullptr;
        obj.mNodePool = nullptr;
        obj.mSize = 0;
    }

    constexpr LinkedList& operator=(const LinkedList& obj) noexcept
    {
        if (this == &obj)
        {
            return *this;
        }
        deleteAll(mHead);
        mHead = nullptr;
        mTail = nullptr;
        mNodePool = obj.mNodePool;
        mSize = 0;
        for (const T& o : obj)
        {
            push_back(o);
        }
        return *this;
    }

    constexpr LinkedList& operator=(LinkedList&& obj) noexcept
    {
        deleteAll(mHead);
        mHead = obj.mHead;
        mTail = obj.mTail;
        mNodePool = obj.mNodePool;
        mSize = obj.mSize;
        obj.mHead = nullptr;
        obj.mTail = nullptr;
        obj.mNodePool = nullptr;
        obj.mSize = 0;
        return *this;
    }

    constexpr IObjectPool<Node<T>>* pool() const noexcept
    {
        return mNodePool;
    }

    constexpr IObjectPool<Node<T>>*& pool() noexcept
    {
        return mNodePool;
    }

    constexpr unsigned size() const noexcept
    {
        return mSize;
    }

    constexpr T* at(unsigned n) const noexcept
    {
        for (Node<T>* curr { mHead }; curr != nullptr; curr = curr->next())
        {
            if (n == 0)
            {
                return &curr->data();
            }
            n--;
        }
        return nullptr;
    }

    constexpr T* operator[](unsigned n) const noexcept
    {
        for (Node<T>* curr { mHead }; curr != nullptr; curr = curr->next())
        {
            if (n == 0)
            {
                return &curr->data();
            }
            n--;
        }
        return nullptr;
    }

    constexpr T* front() const noexcept
    {
        if (mHead != nullptr)
        {
            return &mHead->data();
        }
        return nullptr;
    }

    constexpr T* back() const noexcept
    {
        if (mTail != nullptr)
        {
            return &mTail->data();
        }
        return nullptr;
    }

    constexpr NodeIter<T> begin() const noexcept
    {
        return NodeIter<T> { mHead };
    }

    constexpr NodeIter<T> end() const noexcept
    {
        return NodeIter<T> { nullptr };
    }

    constexpr bool contains(const T& obj) const noexcept
    {
        for (const T& o : *this)
        {
            if (obj == o)
            {
                return true;
            }
        }
        return false;
    }

    constexpr bool push_front(const T& data) noexcept
    {
        Node<T>* node = mNodePool->allocate();
        if (node == nullptr)
        {
            return false;
        }
        // Node might be dirty!
        node->data() = data;
        node->next() = nullptr;
        node->prev() = nullptr;

        if (mHead == nullptr)
        {
            mHead = mTail = node;
            mSize++;
            return true;
        }
        node->next() = mHead;
        node->next()->prev() = node;
        mHead = node;
        mSize++;
        return true;
    }

    constexpr bool push_back(const T& data) noexcept
    {
        Node<T>* node = mNodePool->allocate();
        if (node == nullptr)
        {
            return false;
        }
        // Node might be dirty!
        node->data() = data;
        node->next() = nullptr;
        node->prev() = nullptr;

        if (mHead == nullptr)
        {
            mHead = mTail = node;
            mSize++;
            return true;
        }
        node->prev() = mTail;
        node->prev()->next() = node;
        mTail = node;
        mSize++;
        return true;
    }

    constexpr T* pop_front() noexcept
    {
        if (mHead == nullptr)
        {
            // invalid pop
            return nullptr;
        }
        if (mHead->next() == nullptr)
        {
            // pop head
            T* result = &mHead->data();
            mNodePool->deallocate(mHead);
            mHead = nullptr;
            mTail = nullptr;
            mSize--;
            return result;
        }

        // pop the first node
        T* result = &mHead->data();
        Node<T>* curr = mHead;
        mHead = mHead->next();
        mHead->prev() = nullptr;
        mNodePool->deallocate(curr);
        mSize--;
        return result;
    }

    constexpr T* pop_back() noexcept
    {
        if (mHead == nullptr)
        {
            // invalid pop
            return nullptr;
        }
        if (mHead->next() == nullptr)
        {
            // pop head
            T* result = &mHead->data();
            mNodePool->deallocate(mHead);
            mHead = nullptr;
            mTail = nullptr;
            mSize--;
            return result;
        }

        // pop the last node
        T* result = &mTail->data();
        Node<T>* curr = mTail;
        mTail = mTail->prev();
        mTail->next() = nullptr;
        mNodePool->deallocate(curr);
        mSize--;
        return result;
    }

    /* Needs a pool to allocate from. Does not manipulate the LinkedList,
     * creates a new LinkedList with the provided pool.
     */
    template<typename U>
    constexpr LinkedList<U> map(U (*func)(T), IObjectPool<Node<U>>* pool) const noexcept
    {
        // TODO: remove stack allocation
        LinkedList<U> result;
        result.pool() = pool;

        for (const T& o : *this)
        {
            result.push_back(func(o));
        }

        return result;
    }

    // TODO: export print methods into friend methods in a separate file
    // Only use it with NULL-terminated strings
    constexpr void printStrs(FILE* stream) const noexcept
    {
        fputs("[", stream);
        for (Node<T>* curr { mHead }; curr != nullptr; curr = curr->next())
        {
            fprintf(stream, "%s", static_cast<char*>(curr->data().data()));
            if (curr->next() != nullptr)
            {
                fputs(", ", stream);
            }
        }
        fputs("]", stream);
    }

    // Prints the numbers as int (%d)
    constexpr void printNums(FILE* stream) const noexcept
    {
        fputs("[", stream);
        for (Node<T>* curr { mHead }; curr != nullptr; curr = curr->next())
        {
            fprintf(stream, "%d", curr->data());
            if (curr->next() != nullptr)
            {
                fputs(", ", stream);
            }
        }
        fputs("]", stream);
    }
private:
    Node<T>* mHead { nullptr };
    Node<T>* mTail { nullptr };
    /* Changed mNodePool from a reference to a pointer
     * in order to have a default constructor.
     */
    IObjectPool<Node<T>>* mNodePool { nullptr };
    unsigned mSize { 0 };

    /* Deletes all nodes following the argument and the
     * argument itself.
     */
    constexpr void deleteAll(Node<T>* const node) noexcept
    {
        if (node == nullptr)
        {
            return;
        }
        // pointer allocation in runtime is okay
        Node<T>* next { node->next() };
        mNodePool->deallocate(node);
        // tail recursive, suitable for optimization
        deleteAll(next);
    }
};
