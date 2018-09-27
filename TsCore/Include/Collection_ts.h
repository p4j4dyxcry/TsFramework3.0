#pragma once
#include "Common.h"
namespace TS
{
    /**
     * \brief �R���N�V�����N���X
     */
    template<typename T>
    class Collection
    {
    public:

        /**
        * \brief �R���X�g���N�^
        * \param capacity�@�������m�ۗ�
        */
        Collection(size_t capacity);

        /**
        * \brief �T�C�Y��ύX�@�m�ۍς݃������͉������Ȃ�
        *        ���V�����������ꂽ�C���X�^���X�̓R���X�g���N�^���Ă΂�ĂȂ��̂Œ���
        * \param size�@�V�����R���N�V�����̃T�C�Y
        */
        Collection<T>& Resize(size_t size);


        /**
         * \brief �v�f��S�č폜����
         */
        Collection<T>& Clear();


        /**
        * \brief �v�f���󂩂ǂ���
        */
        bool IsEmpty() const;

        /**
         * \brief ���������m�ۂ���A�m�ۍςݗ̈悪�\���ȏꍇ�͉������Ȃ�
         * \param capacity �������m�ۗ�
         */
        Collection<T>& Reserve(size_t capacity);

        /**
         * \brief �v�f��ǉ�����
         */
        Collection<T>& Add(const T& value);


        /**
         * \brief �R���N�V�������̂Q�̗v�f�����ւ���
         *        �Ăяo���̓R�s�[�R���X�g���N�^�Ăяo�����s����
         * \param index1 
         * \param index2 
         */
        Collection<T>& Swap(size_t index1, size_t index2);

        /**
         * \brief �v�f���w��C���f�b�N�X�ɑ}������
         */
        Collection<T>& Insert(size_t index, const T& value);

        /**
         * \brief �������q��p���ėv�f��ǉ�����
         */
        Collection<T>& AddRange(const std::initializer_list<T>& items);

        /**
         * \brief �v�f��ǉ�����
         */
        Collection<T>& AddRange(const Collection<T>& items);

        /**
         * \brief �ŏ��Ɍ��������v�f���폜����
         */
        Collection<T>& Remove(const T& value);

        /**
         * \brief �C���f�b�N�X�ŗv�f���폜����
         */
        Collection<T>& RemoveAt(size_t index);

        /**
         * \brief �w��͈̗͂v�f���폜����
         */
        Collection<T>& RemoveRange(size_t start, size_t count);

        /**
         * \brief �v�f�����擾����
         */
        size_t Length() const;


        /**
         * \brief �z��̐擪�|�C���^���擾����
         */
        T*& Data();
        const T*& Data() const;

    public:
        Collection();

        Collection(const Collection<T>& c);

        Collection(const std::initializer_list<T>& c);

        Collection(Collection<T>&& c) noexcept;

        Collection<T>& operator =(const Collection<T>& c);

        Collection<T>& operator =(Collection<T>&& c) noexcept;

        virtual ~Collection();

        T& operator[](size_t index);

        const T& operator[](size_t index) const;
    private:

        //! �O�l��
        void pack_front(size_t start, size_t end);

        //! ���l��
        void pack_back(size_t start, size_t end);

        void copy_data(size_t start , size_t count , T* buffer);

        size_t _capacity;
        size_t _size;
        T*   _data;
    };
}
#include "Collection_ts.hpp"