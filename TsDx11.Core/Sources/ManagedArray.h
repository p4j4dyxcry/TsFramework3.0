#pragma once

namespace TS
{
    /**
    * \brief �������Ǘ��������I�ɍs���z��N���X
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
        * \brief �v�f�����擾����
        * \return �v�f��
        */
        size_t Length() const;

        /**
         * \brief �v�f���擾����
         * \return �v�f
         */
        T*& Data();

        ManagedArray<T> Join(const T& data)
        {
            ManagedArray<T> result(_size + 1);
            for(size_t i = 0 ; i < _size; ++i)
            {
                result[i] = _data[i];
            }
            result[_size] = data;
            return result;
        }


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
    protected:
        void Release();

        void AddRef(RefCounter*);

    protected:
        T * _data;
        size_t _size;
    };
}
#include "ManagedArray.hpp"