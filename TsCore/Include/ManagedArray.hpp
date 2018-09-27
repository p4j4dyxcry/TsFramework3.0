#pragma once
#include "New.h"
namespace TS
{    
    template <typename T>
    ManagedArray<T>::~ManagedArray()
    {
        Release();
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(size_t sz) : Array<T>(TS_NEWARRAY(T,sz),sz),_refarenceConter(nullptr)
    {
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(const ManagedArray<T>& ref) : Array<T>(nullptr, 0), _refarenceConter(nullptr)
    {
        if (this != &ref)
            copy_ref_internal(ref._data, ref._size, ref._refarenceConter);
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(const ManagedArray<T>&& ref) noexcept : Array<T>(nullptr, 0), _refarenceConter(nullptr)
    {
        if (this != &ref)
            copy_ref_internal(ref._data, ref._size, ref._refarenceConter);
    }

    template <typename T>
    ManagedArray<T>& ManagedArray<T>::operator=(const ManagedArray<T>& ref)
    {
        if(this != &ref)
            copy_ref_internal(ref._data, ref._size, ref._refarenceConter);
        return *this;
    }

    template <typename T>
    ManagedArray<T>& ManagedArray<T>::operator=(ManagedArray<T>&& ref) noexcept
    {
        if (this != &ref)
            copy_ref_internal(ref._data, ref._size, ref._refarenceConter);
        return *this;
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
            TS_DELETE(this->_data);
            this->_data = nullptr;
            this->_size = 0;
        }
        TS_DELETE(_refarenceConter);
        _refarenceConter = nullptr;
    }

    template<typename T>
    void ManagedArray<T>::AddRef(RefCounter* pRefConter)
    {
        _refarenceConter = pRefConter;
        if (_refarenceConter == nullptr)
            _refarenceConter = TS_NEW(RefCounter)();
        _refarenceConter->AddRef();
    }

    template <typename T>
    void ManagedArray<T>::copy_ref_internal(T* data, size_t sz, RefCounter* pRef)
    {
        Release();

        this->_data = data;
        this->_size = sz;

        AddRef(pRef);
    }
}