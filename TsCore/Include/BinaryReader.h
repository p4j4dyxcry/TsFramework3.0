#pragma once

#include "ManagedArray.h"

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
        Collection<T> ReadArray(unsigned dataCount);

        template<typename T>
        BinaryReader& ReadArray(T* datas, unsigned dataCount);

        /**
         * \brief �ŏ��Ɍ�������'\0'�܂ł𕶎���Ƃ��ēǂݍ��݁A�ǂݍ��񂾕��J�����g��i�߂�
         * \return �ǂݍ���String
         */
        StringA ReadString();

        /**
         * \brief �J�����g�ʒu���ړ������܂�
         * \param offset �ړ�������o�C�i��index
         * \return ���g���߂�l�Ƃ��ĕԋp�����
         */
        BinaryReader& Seek(unsigned offset);

        BinaryReader& Skip(unsigned offset);
        
        bool Eof() const
        {
            return _current == _binary.Length() - 1;
        }
    protected:
        unsigned _current;
        Binary _binary;
    };
}
#include "BinaryReader.hpp"