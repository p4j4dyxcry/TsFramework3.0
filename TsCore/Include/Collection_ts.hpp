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

    template <typename T>
    bool Collection<T>::IsEmpty() const
    {
        return _size == 0;
    }

    template <typename T>
    Collection<T>& Collection<T>::Add(const T& value)
    {
        if (_size + 1 < _capacity)
            _data[_size++] = value;
        else
        {
            const size_t newCapacity = _capacity == 0 ? 2 : _capacity * 2;
            Reserve(newCapacity);
            _data[_size++] = value;
        }

        return *this;
    }

    template <typename T>
    Collection<T>& Collection<T>::Swap(size_t index1, size_t index2)
    {
        if (index1 >= _size || index2 >= _size)
            throw ExceptionMessage::IndexOfOutrange;

        T temp = _data[index1];
        _data[index1] = _data[index2];
        _data[index2] = temp;
        return *this;
    }

    template <typename T>
    Collection<T>& Collection<T>::Insert(size_t index, const T& value)
    {
        if (index >= _size)
            throw ExceptionMessage::IndexOfOutrange;
        {
            Reserve(_size + 1);
            pack_back(index, index + 1);
            _data[index] = value;
            _size += 1;
        }
        return *this;
    }

    template <typename T>
    Collection<T>& Collection<T>::AddRange(const std::initializer_list<T>& items)
    {
        if (items.size() == 0)
            return *this;
        if (_size + items.size() <= _capacity)
        {
            copy_data(_size, items.size(), (T*)items.begin());
            _size += items.size();
        }
        else
        {
            size_t newCapacity = _capacity == 0 ? 1 : _capacity;

            while (newCapacity < _size + items.size())
                newCapacity *= 2;
            Reserve(newCapacity);

            copy_data(_size, items.size(), (T*)items.begin());
            _size += items.size();
        }
        return *this;
    }

    template <typename T>
    Collection<T>& Collection<T>::AddRange(const Collection<T>& items)
    {
        if (items._size == 0)
            return *this;

        if (_size + items._size <= _capacity)
        {
            copy_data(_size, items._size, items._data);
            _size += items._size;
        }
        else
        {
            size_t newCapacity = _capacity == 0 ? 1 : _capacity;

            while (newCapacity < _size + items._size)
                newCapacity *= 2;
            Reserve(newCapacity);
            copy_data(_size, items._size, items._data);
            _size += items._size;
        }

        return *this;
    }

    template <typename T>
    Collection<T>& Collection<T>::Remove(const T& value)
    {
        for (size_t i = 0; i < _size; ++i)
        {
            if (_data[i] == value)
            {
                RemoveRange(i, 1);
                return *this;
            }
        }

        throw ExceptionMessage::NotFound;
    }

    template <typename T>
    Collection<T>& Collection<T>::RemoveAt(size_t index)
    {
        return RemoveRange(index, 1);
    }

    template <typename T>
    Collection<T>& Collection<T>::RemoveRange(size_t start, size_t count)
    {
        const size_t end = start + count;

        if (_size < end)
            throw ExceptionMessage::IndexOfOutrange;
        {
            pack_front(start, end);
            _size -= count;
        }

        return *this;
    }

    template <typename T>
    size_t Collection<T>::Length() const
    {
        return _size;
    }

    template <typename T>
    T*& Collection<T>::Data()
    {
        return _data;
    }

    template <typename T>
    const T*& Collection<T>::Data() const
    {
        return const_cast<const T*&>(_data);
    }

    template <typename T>
    Collection<T>::Collection(): _capacity(0), _size(0), _data(nullptr)
    {
    }

    template <typename T>
    Collection<T>::Collection(size_t capacity): Collection()
    {
        Reserve(capacity);
    }

    template <typename T>
    Collection<T>::Collection(const Collection<T>& c): Collection()
    {
        Resize(0);
        AddRange(c);
    }

    template <typename T>
    Collection<T>::Collection(const std::initializer_list<T>& c): Collection()
    {
        Resize(0);
        AddRange(c);
    }

    template <typename T>
    Collection<T>::Collection(Collection<T>&& c) noexcept: Collection()
    {
        Resize(0);
        AddRange(c);
    }

    template <typename T>
    Collection<T>& Collection<T>::operator=(const Collection<T>& c)
    {
        if( this != &c)
        {
            Resize(0);
            AddRange(c);
        }
        return *this;
    }

    template <typename T>
    Collection<T>& Collection<T>::operator=(Collection<T>&& c) noexcept
    {
        if (this != &c)
        {
            Resize(0);
            AddRange(c);
        }
        return *this;
    }

    template <typename T>
    Collection<T>::~Collection()
    {
        if (_capacity != 0)
            TS_DELETE(_data);
    }

    template <typename T>
    T& Collection<T>::operator[](size_t index)
    {
        if (index >= _size) 
            throw ExceptionMessage::IndexOfOutrange;

        return _data[index];
    }

    template <typename T>
    const T& Collection<T>::operator[](size_t index) const
    {
        if (index >= _size)
            throw ExceptionMessage::IndexOfOutrange;

        return _data[index];
    }

    //! データを後ろに詰める
    template <typename T>
    void Collection<T>::pack_front(size_t start, size_t end)
    {
        for (size_t i = 0; i < _size - end; ++i)
            _data[start + i] = _data[end + i];
    }

    //! データを前に詰める
    template <typename T>
    void Collection<T>::pack_back(size_t start, size_t end)
    {
        for (size_t i = 0; i < _size - end; ++i)
            _data[_size - i + start] = _data[_size - i];
    }

    template<typename T>
    inline void Collection<T>::copy_data(size_t start, size_t count, T * buffer)
    {
        for (size_t i = 0; i < count; ++i)
            _data[start + i] = buffer[i];
    }

    template <typename T>
    Collection<T>& Collection<T>::Reserve(size_t capacity)
    {
        if (_capacity < capacity)
        {
            _capacity = capacity;
            T* old_data = _data;
            _data = TS_NEWARRAY(T, _capacity);

            if (old_data != nullptr)
            {
                copy_data(0, _size, old_data);
                TS_DELETE(old_data);
            }
        }
        return *this;
    }

    template<typename T>
    T* begin(Collection<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    T* end(Collection<T>& s)
    {
        return { s.Data() + s.Length() };
    };

    template<typename T>
    const T* begin(const Collection<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    const T* end(const Collection<T>& s)
    {
        return { s.Data() + s.Length() };
    };
}