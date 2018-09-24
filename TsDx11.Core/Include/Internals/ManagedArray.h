#pragma once

namespace TS
{
	template<typename T>
	struct Array
	{
	public:
		Array(const T* data, size_t sz) :_data(const_cast<T*>(data)), _size(sz) {  }
		//Array(T* data, size_t sz) :_data(data), _size(sz) {  }

		/**
		* \brief 要素を取得する
		*/
		T* Data() { return _data; }
		const T* Data()const { return _data; }
		
		/**
		* \brief 要素数を取得する
		*/
		size_t Length()const { return _size; }

		operator void*()const { return _data; }
		operator const void *() const { return _data; }
		operator T*() { return _data; }
		operator const T*() const { return _data; }

	protected:
		T * _data;
		size_t _size;
	};

    /**
    * \brief メモリ管理を自動的に行う配列クラス
    * \tparam T
    */
    template<typename T>
    struct ManagedArray : Array<T>
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
         * \brief 要素を結合した新しい配列を作成する
         * \param data 新しい要素
         * \return 作成された新しい配列
         */
        ManagedArray<T> Join(const T& data);

        ManagedArray();
        ManagedArray(const T* data , size_t sz , RefCounter* pRefCounter = nullptr);
        ManagedArray(size_t sz);
        ManagedArray(const ManagedArray<T>& ref);
        ManagedArray(const ManagedArray<T>&& ref) noexcept;
        virtual ~ManagedArray();
        ManagedArray<T>& operator =(const ManagedArray<T>& ref);
        ManagedArray<T>& operator =(ManagedArray<T>&& ref) noexcept;

    protected:
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