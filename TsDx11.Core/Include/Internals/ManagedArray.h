#pragma once

namespace TS
{
	template<typename T>
	struct Array
	{
	public:
		Array(const T* data, size_t sz) :_data(data), _size(sz) {  }

		/**
		* \brief �v�f���擾����
		*/
		T* Data() { return _data; }
		const T* Data()const { return _data; }
		
		/**
		* \brief �v�f�����擾����
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
    * \brief �������Ǘ��������I�ɍs���z��N���X
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
         * \brief �v�f�����������V�����z����쐬����
         * \param data �V�����v�f
         * \return �쐬���ꂽ�V�����z��
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