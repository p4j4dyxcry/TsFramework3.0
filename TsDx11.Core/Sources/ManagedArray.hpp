#pragma once
namespace TS
{


    template <typename T>
    ManagedArray<T>::~ManagedArray()
    {
        Release();
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(const T* data, size_t sz, RefCounter* pRefCounter)
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
    ManagedArray<T>::ManagedArray(size_t sz) : ManagedArray(new T[sz], sz)
    {
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(const ManagedArray<T>& ref) : ManagedArray<T>(
        ref._data, ref._size, ref._refarenceConter)
    {
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(const ManagedArray<T>&& ref) noexcept : ManagedArray<T>(
        ref._data, ref._size, ref._refarenceConter)
    {
    }

    template <typename T>
    ManagedArray<T>& ManagedArray<T>::operator=(const ManagedArray<T>& ref)
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
    ManagedArray<T>& ManagedArray<T>::operator=(ManagedArray<T>&& ref) noexcept
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
    ManagedArray<T>::operator T*()
    {
        return _data;
    }

    template <typename T>
    ManagedArray<T>::operator const T*() const
    {
        return _data;
    }

    template <typename T>
    void ManagedArray<T>::Release()
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
    T*& ManagedArray<T>::Data()const
    {
        return _data;
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(): _data(nullptr), _size(0), _refarenceConter(nullptr)
    {
    }

    template <typename T>
    size_t ManagedArray<T>::Length() const
    {
        return _size;
    }
}