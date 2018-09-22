#pragma once

namespace TS
{
    /**
    * \brief メモリ管理を自動的に行う配列クラス
    * \tparam T
    */
    template<typename T>
    struct ManagedArray 
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
        T*& Data();


        /**
         * \brief 要素を結合した新しい配列を作成する
         * \param data 新しい要素
         * \return 作成された新しい配列
         */
        ManagedArray<T> Join(const T& data);

        ManagedArray();
        ManagedArray(T* data , size_t sz , RefCounter* pRefCounter = nullptr);
        ManagedArray(size_t sz);
        ManagedArray(const ManagedArray<T>& ref);
        ManagedArray(const ManagedArray<T>&& ref) noexcept;
        virtual ~ManagedArray();
        ManagedArray<T>& operator =(const ManagedArray<T>& ref);
        ManagedArray<T>& operator =(ManagedArray<T>&& ref) noexcept;

        operator void*()const;
        operator const void *() const;
        operator T*();
        operator const T*() const;

		using iterator = T*;

    protected:
        void Release();

        void AddRef(RefCounter*);

    protected:
        T * _data;
        size_t _size;
    };

    template<typename T>
    T* begin(ManagedArray<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    T* end(ManagedArray<T>& s)
    {
        return { s.Data() + s.Length() };
    };

    template<typename T>
    T* begin(const ManagedArray<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    T* end(const ManagedArray<T>& s)
    {
        return { s.Data() + s.Length() };
    };


}
#include "ManagedArray.hpp"