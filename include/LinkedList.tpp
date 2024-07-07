#pragma once

#include <cstdio>

#include "IObjectPool.tpp"
#include "Node.tpp"
#include "NodeIter.tpp"

template<typename T>
class LinkedList
{
public:
    constexpr LinkedList() noexcept = default;
    constexpr LinkedList(IObjectPool<Node<T>>* pool) noexcept
        : mNodePool{ pool }
    {
    }
    constexpr ~LinkedList() noexcept
    {
        deleteAll(mHead);
    }
    constexpr LinkedList(const LinkedList& obj) noexcept
        : mNodePool{ obj.mNodePool }
    {
        for (unsigned i{ 0 }; i < obj.size(); ++i)
        {
            push(*obj[i]);
        }
    }
    constexpr LinkedList(LinkedList&& obj) noexcept
        : mHead{ obj.mHead }
        , mNodePool{ obj.mNodePool }
        , mSize{ obj.mSize }
    {
    }
    constexpr LinkedList& operator=(const LinkedList& obj) noexcept
    {
        if (this == &obj)
        {
            return *this;
        }
        deleteAll(mHead);
        mHead = nullptr;
        mNodePool = obj.mNodePool;
        mSize = 0;
        for (unsigned i{ 0 }; i < obj.size(); ++i)
        {
            push(*obj[i]);
        }
        return *this;
    }
    constexpr LinkedList& operator=(LinkedList&& obj) noexcept
    {
        deleteAll(mHead);
        mHead = obj.mHead;
        mNodePool = obj.mNodePool;
        mSize = obj.mSize;
        return *this;
    }

    constexpr unsigned size() const noexcept
    {
        return mSize;
    }

    constexpr T* at(unsigned n) const noexcept
    {
        for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
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
        for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
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
        for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
        {
            if (curr->next() == nullptr)
            {
                return &curr->data();
            }
        }
        return nullptr;
    }

    constexpr NodeIter<T> begin() const noexcept
    {
        return NodeIter<T>{ mHead };
    }

    constexpr NodeIter<T> end() const noexcept
    {
        return NodeIter<T>{ nullptr };
    }

    constexpr bool push(const T& data) noexcept
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
            mHead = node;
            mSize++;
            return true;
        }
        Node<T>* curr = mHead;
        while (curr->next() != nullptr)
        {
            curr = curr->next();
        }
        curr->next() = node;
        curr->next()->prev() = curr;
        mSize++;
        return true;
    }

    constexpr T* pop() noexcept
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
            mSize--;
            return result;
        }

        // pop the last node
        Node<T>* curr = mHead;
        while (curr->next() != nullptr)
        {
            curr = curr->next();
        }
        curr->prev()->next() = nullptr;
        T* result = &curr->data();
        mNodePool->deallocate(curr);
        mSize--;
        return result;
    }

    /* Needs a pool to allocate from. Does not manipulate the LinkedList,
     * creates a new LinkedList with the provided pool.
     */
    template<typename U>
    constexpr LinkedList<U> map(U (*func)(T),
                                IObjectPool<Node<U>>& pool) const noexcept
    {
        // TODO: remove stack allocation
        LinkedList<U> result{ &pool };

        for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
        {
            if (!result.push(func(curr->data())))
            {
                puts("failed to push");
            }
        }

        return result;
    }

    constexpr bool contains(const T& obj) const noexcept
    {
        for (auto&& o : *this)
        {
            if (obj == o)
            {
                return true;
            }
        }
        return false;
    }

    // Only use it with NULL-terminated strings
    constexpr void printStrs(FILE* stream) const noexcept
    {
        fputs("[", stream);
        for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
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
        for (Node<T>* curr{ mHead }; curr != nullptr; curr = curr->next())
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
    Node<T>* mHead{ nullptr };
    /* Changed mNodePool from a reference to a pointer
     * in order to have a default constructor.
     */
    IObjectPool<Node<T>>* mNodePool;
    unsigned mSize{ 0 };

    /* Deletes all nodes following the argument and the
     * argument itself.
     */
    constexpr void deleteAll(Node<T>* const node) noexcept
    {
        if (node == nullptr)
        {
            return;
        }
        Node<T>* next{ node->next() };
        mNodePool->deallocate(node);
        // tail recursive, suitable for optimization
        deleteAll(next);
    }
};
