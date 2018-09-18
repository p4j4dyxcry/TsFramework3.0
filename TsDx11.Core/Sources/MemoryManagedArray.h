#pragma once

namespace TS
{
    /**
    * \brief �������Ǘ��������I�ɍs���z��N���X
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
        * \brief �v�f�����擾����
        * \return �v�f��
        */
        size_t Length() const;

        /**
         * \brief �v�f���擾����
         * \return �v�f
         */
        T* Data()const;

        MemoryManagedArray():_data(nullptr),_size(0),_refarenceConter(nullptr)
        {
        }

        MemoryManagedArray(T* data , size_t sz , RefCounter* pRefCounter = nullptr);
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
