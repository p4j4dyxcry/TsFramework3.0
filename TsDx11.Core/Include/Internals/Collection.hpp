#pragma once
namespace TS
{

    template <typename T>
    Collection<T>& Collection<T>::Resize(size_t size)
    {
        Reserve(size);
        _size = size;

        return *this;
    }

    template <typename T>
    Collection<T>& Collection<T>::Clear()
    {
        Resize(0);
        return *this;
    }

    template<typename T>
    PointerIterater<T> begin(Collection<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    PointerIterater<T> end(Collection<T>& s)
    {
        return { s.Data() + s.size() };
    };

    template<typename T>
    PointerIterater<T> begin(const Collection<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    PointerIterater<T> end(const Collection<T>& s)
    {
        return { s.Data() + s.size() };
    };
}