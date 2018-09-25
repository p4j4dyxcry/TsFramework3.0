#pragma once

namespace TS
{

    /**
     * \brief ������N���X
     */
    template<typename T>
    class String : public ManagedArray<T>
    {
    public:
        /**
         * \brief �R���X�g���N�^
         * \param str ����������
         */
        String(const T* str );

        /**
         * \brief �������ꂽ�T�u������̍쐬
         * \param pos �J�nIndex
         * \param npos  pos ����� �J�E���g , 0�̏ꍇ�͏I�[�܂łƂ��Ĉ���
         * \return �������ꂽ�T�u������
         */
        String<T> SubString(unsigned pos, unsigned npos = 0) const;

        /**
         * \brief �����񌟍�
         * \param _pattern ����������
         * \return ��������������ւ�Index�A������Ȃ������ꍇ�� -1
         */
        int Find(const T* _pattern) const;

        /**
         * \brief ��������
         * \param _pattern ����������
         * \return ��������������ւ�Index�A������Ȃ������ꍇ�� -1
         */
        int Find(const T& _pattern) const;

        /**
         * \brief �����񌟍�
         * \param _pattern ����������
         * \return ��������������ւ̍Ō��Index�A������Ȃ������ꍇ�� -1
         */
        int Rfind(const T* _pattern) const;

        /**
         * \brief ��������
         * \param _pattern ����������
         * \return ��������������ւ�Index�A������Ȃ������ꍇ�� -1
         */
        int Rfind(const T& _pattern) const;

        /**
         * \brief �����u��
         * \param original �u�����镶��
         * \param _new �u������镶�� 
         * \return �u�����ꂽ������
         */
        String<T> Replace(T original, T _new) const;

        /**
         * \brief ������u��
         * \param start �u������J�n�C���f�b�N�X
         * \param size �u������J�n�C���f�b�N�X����̃J�E���g
         * \param str �u����̕����� size�𒴂��镶��������e�����
         * \return �u�����ꂽ������
         */
        String<T> Replace(unsigned start, unsigned size, const String<T>& str) const;

        /**
         * \brief ������u��
         * \param original �u�����镶��
         * \param _new �u������镶��
         * \return �u�����ꂽ������
         */
        String<T> Replace(const T* original, const T* _new) const;
        
        /**
         * \brief �Ώۂ̕����񂪊܂܂�邩���ׂ�
         * \param str �Ώۂ̕�����
         * \return �܂܂��ꍇ��true
         */
        bool Contain(const T* str) const;
               
        /**
         * \brief �����񂪋�null�𔻒�
         * \return null or �󕶎��Ȃ� true
         */
        bool IsNullOrEmpty() const;
        
        //! Static-------------------------------------------------------------
        /**
        * \brief �������𒲂ׂ�
        * \param str �Ώۂ̕�����
        * \return ������
        */
        static size_t StringLength(const T* str);
        
        /**
         * \brief �������當������쐬����
         * \param format ����
         * \param ... 
         * \return �������ꂽ������
         */
        static String<T> Format(const T* format, ...) ;

        // operator
    public:
        using ManagedArray<T>::ManagedArray;

        bool operator ==(const String<T>& string) const;
        bool operator !=(const String<T>& string) const;

        bool operator ==(const T* string) const;
        bool operator !=(const T* string) const;

        String<T>& operator =(const T* str);
        String<T> operator  +(const T* str) const;
        String<T>& operator +=(const T* str);

        String():ManagedArray<T>(){}
        String(const String<T>& ref);
        String(const String<T>&& ref) noexcept;

        String<T>& operator =(const String<T>& ref);
        String<T>& operator =(String<T>&& ref) noexcept;
    };

    using StringA = String<char>;
    using StringW = String<wchar_t>;
}

#include "String_ts.hpp"