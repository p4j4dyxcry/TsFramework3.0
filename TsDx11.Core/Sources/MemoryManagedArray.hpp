#pragma once
namespace TS
{


    template <typename T>
    MemoryManagedArray<T>::~MemoryManagedArray()
    {
        Release();
    }

    template <typename T>
    MemoryManagedArray<T>::MemoryManagedArray(T* data, size_t sz, RefCounter* pRefCounter)
    {
        _data = data;
        _size = sz;
        _refarenceConter = pRefCounter;
        if (_refarenceConter == nullptr)
            _refarenceConter = new RefCounter();

        if (_refarenceConter != nullptr)
            _refarenceConter->AddRef();
    }

    template <typename T>
    MemoryManagedArray<T>::MemoryManagedArray(size_t sz) : MemoryManagedArray(new T[sz], sz)
    {
    }

    template <typename T>
    MemoryManagedArray<T>::MemoryManagedArray(const MemoryManagedArray<T>& ref) : MemoryManagedArray<T>(
        ref._data, ref._size, ref._refarenceConter)
    {
    }

    template <typename T>
    MemoryManagedArray<T>::MemoryManagedArray(const MemoryManagedArray<T>&& ref) noexcept : MemoryManagedArray<T>(
        ref._data, ref._size, ref._refarenceConter)
    {
    }

    template <typename T>
    MemoryManagedArray<T>& MemoryManagedArray<T>::operator=(const MemoryManagedArray<T>& ref)
    {
        Release();

        this->_data = ref._data;
        this->_size = ref._size;

        _refarenceConter = ref._refarenceConter;
        if (_refarenceConter != nullptr)
            _refarenceConter->AddRef();
        return *this;
    }

    template <typename T>
    MemoryManagedArray<T>& MemoryManagedArray<T>::operator=(MemoryManagedArray<T>&& ref) noexcept
    {
        Release();

        this->_data = ref._data;
        this->_size = ref._size;

        _refarenceConter = ref._refarenceConter;
        if (_refarenceConter != nullptr)
            _refarenceConter->AddRef();
        return *this;
    }

    template <typename T>
    MemoryManagedArray<T>::operator T*()
    {
        return _data;
    }

    template <typename T>
    MemoryManagedArray<T>::operator const T*() const
    {
        return _data;
    }

    template <typename T>
    void MemoryManagedArray<T>::Release()
    {
        if (_refarenceConter == nullptr)
            return;
        _refarenceConter->SubRef();
        if (_refarenceConter->GetRef() != 0)
            return;

        if(this->_data != nullptr)
        {
            delete _data;
            _data = nullptr;
            _size = 0;
        }
        delete _refarenceConter;
        _refarenceConter = nullptr;
    }

    template <typename T>
    T* MemoryManagedArray<T>::Data()const
    {
        return _data;
    }

    template <typename T>
    size_t MemoryManagedArray<T>::Length() const
    {
        return _size;
    }
}