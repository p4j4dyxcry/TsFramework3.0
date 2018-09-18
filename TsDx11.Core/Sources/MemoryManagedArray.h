#pragma once

namespace TS
{
    /**
    * \brief メモリ管理を自動的に行う配列クラス
    * \tparam T
    */
    template<typename T>
    struct MemoryManagedArray 
    {
    protected:

        class RefCounter
        {
        private:
            unsigned m_referenceCount;
        public:
            RefCounter()
                : m_referenceCount(0) {}
            RefCounter* AddRef() { ++(m_referenceCount); return this; }
            RefCounter* SubRef() { --(m_referenceCount); return this; }
            unsigned    GetRef()const { return m_referenceCount; }
        };

        RefCounter * _refarenceConter{};

    public:
        /**
        * \brief 要素数を取得する
        * \return 要素数
        */
        size_t Length() const;

        /**
         * \brief 要素を取得する
         * \return 要素
         */
        T* Data()const;

        MemoryManagedArray();
        MemoryManagedArray(const T* data , size_t sz , RefCounter* pRefCounter = nullptr);
        MemoryManagedArray(size_t sz);
        MemoryManagedArray(const MemoryManagedArray<T>& ref);
        MemoryManagedArray(const MemoryManagedArray<T>&& ref) noexcept;
        virtual ~MemoryManagedArray();
        MemoryManagedArray<T>& operator =(const MemoryManagedArray<T>& ref);
        MemoryManagedArray<T>& operator =(MemoryManagedArray<T>&& ref) noexcept;

        operator T*();
        operator const T*() const;
    protected:
        void Release();
    protected:
        T * _data;
        size_t _size;
    };
}
#include "MemoryManagedArray.hpp"