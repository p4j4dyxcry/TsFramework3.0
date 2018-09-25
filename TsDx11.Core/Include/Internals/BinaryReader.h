#pragma once

namespace TS
{
    using Binary = Array<unsigned char>;

    /**
     * \brief �o�C�i���ǂݍ��݂̂��߂̃��[�e�B���e�B
     */
    class BinaryReader
    {
    public:
        /**
         * \brief �R���X�g���N�^
         * \param binary �ǂݍ��ނ��s���o�C�i��
         */
        BinaryReader(const Binary& binary);
        
        /**
         * \brief �J�����g����T�^�̃T�C�Y���f�[�^��ǂݍ��݁A�ǂݍ��񂾕��J�����g��i�߂�
         * \tparam T �f�[�^�̌^
         * \return �ǂݍ��񂾃f�[�^
         */
        template<typename T>
        T Read();

        /**
         * \brief �J�����g����T�^�� * dataCount �̃T�C�Y���f�[�^��ǂݍ��݁A�ǂݍ��񂾕��J�����g��i�߂�
         * \tparam T �f�[�^�̌^
         * \param dataCount �f�[�^�̐�
         * \return �ǂݍ��񂾃f�[�^�z��
         */
        template<typename T>
        ManagedArray<T> ReadArray(unsigned dataCount);

        /**
         * \brief �ŏ��Ɍ�������'\0'�܂ł𕶎���Ƃ��ēǂݍ��݁A�ǂݍ��񂾕��J�����g��i�߂�
         * \return �ǂݍ���String
         */
        String<TsChar> ReadString();

        /**
         * \brief �J�����g�ʒu���ړ������܂�
         * \param offset �ړ�������o�C�i��index
         * \return ���g���߂�l�Ƃ��ĕԋp�����
         */
        BinaryReader& Seek(unsigned offset);
        
        bool Eof() 
        {
            return _current == _binary.Length();
        }
    protected:
        unsigned _current;
        Binary _binary;
    };
}
#include "BinaryReader.hpp"