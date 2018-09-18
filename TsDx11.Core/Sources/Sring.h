#pragma once

namespace TS
{

    /**
     * \brief ������N���X
     */
    class String : public MemoryManagedArray<TsChar>
    {
    public:
        
        /**
         * \brief �R���X�g���N�^
         * \param str ����������
         */
        String(const TsChar* str = _T(""));

        /**
         * \brief �������ꂽ�T�u������̍쐬
         * \param pos �J�nIndex
         * \param npos  pos ����� �J�E���g , 0�̏ꍇ�͏I�[�܂łƂ��Ĉ���
         * \return �������ꂽ�T�u������
         */
        String SubString(unsigned pos, unsigned npos = 0) const;

        /**
         * \brief �����񌟍�
         * \param _pattern ����������
         * \return ��������������ւ�Index�A������Ȃ������ꍇ�� -1
         */
        int Find(const TsChar* _pattern) const;

        /**
         * \brief �����u��
         * \param original �u�����镶��
         * \param _new �u������镶�� 
         * \return �u�����ꂽ������
         */
        String Replace(TsChar original, TsChar _new) const;

        /**
         * \brief ������u��
         * \param start �u������J�n�C���f�b�N�X
         * \param size �u������J�n�C���f�b�N�X����̃J�E���g
         * \param str �u����̕����� size�𒴂��镶��������e�����
         * \return �u�����ꂽ������
         */
        String Replace(unsigned start, unsigned size, const String& str) const;

        /**
         * \brief ������u��
         * \param original �u�����镶��
         * \param _new �u������镶��
         * \return �u�����ꂽ������
         */
        String Replace(const TsChar* original, const TsChar* _new) const;
        
        /**
         * \brief �Ώۂ̕����񂪊܂܂�邩���ׂ�
         * \param str �Ώۂ̕�����
         * \return �܂܂��ꍇ��true
         */
        bool Contain(const TsChar* str) const;
               
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
        static size_t Length(const TsChar* str);
        
        /**
         * \brief �������當������쐬����
         * \param format ����
         * \param ... 
         * \return �������ꂽ������
         */
        static String Format(const TsChar* format, ...) ;

        // operator
    public:
        using MemoryManagedArray<TsChar>::MemoryManagedArray;
        using MemoryManagedArray<TsChar>::operator=;
        bool operator ==(const String& string) const;
        bool operator !=(const String& string) const;
        String& operator =(const TsChar* str);
        String operator  +(const TsChar* str) const;
        String& operator +=(const TsChar* str);
    };
}

