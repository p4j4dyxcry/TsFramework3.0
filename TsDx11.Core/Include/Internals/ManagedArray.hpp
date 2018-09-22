#pragma once
namespace TS
{    
    template <typename T>
    ManagedArray<T>::~ManagedArray()
    {
        Release();
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(T* data, size_t sz, RefCounter* pRefCounter)
        : Array<T>(data,sz),_refarenceConter(pRefCounter)
    {
        AddRef(_refarenceConter);
    }

    template <typename T>
    ManagedArray<T>::ManagedArray(size_t sz) : ManagedArray( TS_NEWARRAY(T,sz), sz , nullptr)
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

        AddRef(ref._refarenceConter);
        return *this;
    }

    template <typename T>
    ManagedArray<T>& ManagedArray<T>::operator=(ManagedArray<T>&& ref) noexcept
    {
        Release();

        this->_data = ref._data;
        this->_size = ref._size;

        AddRef(ref._refarenceConter);
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
    ManagedArray<T> ManagedArray<T>::Join(const T& data)
    {
        ManagedArray<T> result(this->_size + 1);
        for (size_t i = 0; i < this->_size; ++i)
        {
            result[i] = this->_data[i];
        }
        result[this->_size] = data;
        return result;
    }

    template <typename T>
    ManagedArray<T>::ManagedArray():Array<T>(nullptr,0), _refarenceConter(nullptr)
    {
    }
}