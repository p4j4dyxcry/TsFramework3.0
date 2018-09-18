#pragma once

namespace TS
{
    using Binary = ManagedArray<unsigned char>;

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
        String ReadString();

        /**
         * \brief �J�����g�ʒu���ړ������܂�
         * \param offset �ړ�������o�C�i��index
         * \return ���g���߂�l�Ƃ��ĕԋp�����
         */
        BinaryReader& Seek(unsigned offset);

        /**
         * \brief �J�����g����current_to_offset�܂ł̐V�����o�C�i�����[�_���쐬����
         * \param current_to_offset ���݈ʒu����̃T�C�Y
         * \return �쐬���ꂽ�o�C�i�����[�_
         */
        BinaryReader MakeSubReaderFromCurrent(unsigned current_to_offset) const;

        /**
         * \brief �擪���琔����head_to_offeset ���� count �܂ł̐V�����o�C�i�����[�_���쐬����
         * \param head_to_offeset �擪����̊J�n�C���f�b�N�X
         * \param count head_to_offeset����ǂݍ��ރT�C�Y , 0 �̏ꍇ��head_to_offeset����Ō�܂œǂݍ���
         * \return �쐬���ꂽ�o�C�i�����[�_
         */
        BinaryReader MakeSubReaderFromHead(unsigned head_to_offeset, unsigned count = 0) const;
    private:
        unsigned _current;
        Binary _binary;
    };
}
#include "BinaryReader.hpp"