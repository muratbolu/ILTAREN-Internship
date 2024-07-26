#pragma once

#include "iter/NodeIter.tpp"
#include "sds/IObjectPool.tpp"
#include "sds/Node.tpp"

template<typename T>
class LinkedList
{
public:
    constexpr explicit LinkedList(IObjectPool<Node<T>>* pool) noexcept :
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
            pushBack(o);
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
            pushBack(o);
        }
        return *this;
    }

    // Totally buggy and results in double delete.
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

    [[nodiscard]] constexpr unsigned size() const noexcept
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

    constexpr bool pushFront(const T& data) noexcept
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

    constexpr bool pushBack(const T& data) noexcept
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

    constexpr T* popFront() noexcept
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

    constexpr T* popBack() noexcept
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
            result.pushBack(func(o));
        }

        return result;
    }

    // tests for strict equality.
    // operator!= is automatically generated
    constexpr friend bool operator==(const LinkedList<T>& lhs, const LinkedList<T>& rhs) noexcept
    {
        if (lhs.size() != rhs.size())
        {
            return false;
        }
        for (NodeIter<T> itl { lhs.begin() }, itr { rhs.begin() }; itl != lhs.end() && itr != rhs.end(); ++itl, ++itr)
        {
            if (*itl != *itr)
            {
                return false;
            }
        }
        return true;
    }

    constexpr static void swap(Node<T>* n1, Node<T>* n2) noexcept
    {
        auto swapAdj = [](Node<T>* l, Node<T>* r)
        {
            l->next() = r->next();
            r->prev() = l->prev();
            l->prev() = r;
            r->next() = l;
            if (l->next() != nullptr)
            {
                l->next()->prev() = l;
            }
            if (r->prev() != nullptr)
            {
                r->prev()->next() = r;
            }
        };
        if (n1->next() == n2)
        {
            swapAdj(n1, n2);
        }
        else if (n2->next() == n1)
        {
            swapAdj(n2, n1);
        }
        else
        {
            Node<T>* n1Next = n1->next();
            Node<T>* n1Prev = n1->prev();
            n1->next() = n2->next();
            n1->prev() = n2->prev();
            n2->next() = n1Next;
            n2->prev() = n1Prev;

            if (n1->next() != nullptr)
            {
                n1->next()->prev() = n1;
            }
            if (n1->prev() != nullptr)
            {
                n1->prev()->next() = n1;
            }
            if (n2->next() != nullptr)
            {
                n2->next()->prev() = n2;
            }
            if (n2->prev() != nullptr)
            {
                n2->prev()->next() = n2;
            }
        }
    }

    constexpr void sort() noexcept
    {
        if (mHead == nullptr)
        {
            return;
        }
        if (mSize <= 1)
        {
            return;
        }
        for (Node<T>* curr = mHead; curr != nullptr; curr = curr->next())
        {
            for (Node<T>* insert = curr; insert != nullptr && insert->prev() != nullptr && insert->data() < insert->prev()->data();)
            {
                if (insert == mTail)
                {
                    mTail = insert->prev();
                }
                swap(insert, insert->prev());
                if (insert->next() == mHead)
                {
                    mHead = insert;
                }
            }
        }
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
