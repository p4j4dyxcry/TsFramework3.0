#pragma once
namespace TS
{
    template<typename T>
    struct Array
    {
    public:
        Array(const T* data, size_t sz) :_data(const_cast<T*>(data)), _size(sz) {  }

        /**
        * \brief 要素を取得する
        */
        T* Data( )const { return _data; }
        
        /**
        * \brief 要素数を取得する
        */
        size_t Length()const { return _size; }

        T& operator[](unsigned index)
        {
            if (index >= _size)
                throw ExceptionMessage::IndexOfOutrange;

            return _data[index];
        }

        const T& operator[](unsigned index)const
        {
            if (index >= _size)
                throw ExceptionMessage::IndexOfOutrange;

            return _data[index];
        }

    protected:
        T * _data;
        size_t _size;
    };

    class RefCounter
    {
    private:
        unsigned m_referenceCount;
    public:
        RefCounter(): m_referenceCount(0) {}
        RefCounter* AddRef() { ++(m_referenceCount); return this; }
        RefCounter* SubRef() { --(m_referenceCount); return this; }
        unsigned    GetRef()const { return m_referenceCount; }
    };

    /**
    * \brief メモリ管理を自動的に行う配列クラス
    * \tparam T
    */
    template<typename T>
    struct ManagedArray : public Array<T>
    {
    protected:

        RefCounter * _refarenceConter{};

    public:
        ManagedArray(size_t sz);
        ManagedArray(const ManagedArray<T>& ref);
        ManagedArray(const ManagedArray<T>&& ref) noexcept;
        virtual ~ManagedArray();
        ManagedArray<T>& operator =(const ManagedArray<T>& ref);
        ManagedArray<T>& operator =(ManagedArray<T>&& ref) noexcept;

    protected:

        void copy_ref_internal(T* data, size_t sz, RefCounter* pRef);

        void Release();
        void AddRef(RefCounter*);

    };

    template<typename T>
    T* begin(Array<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    T* end(Array<T>& s)
    {
        return { s.Data() + s.Length() };
    };

    template<typename T>
    T* begin(const Array<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    T* end(const Array<T>& s)
    {
        return { s.Data() + s.Length() };
    };

}
#include "ManagedArray.hpp"