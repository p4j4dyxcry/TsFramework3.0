#pragma once

namespace TS
{
    using Binary = MemoryManagedArray<unsigned char>;

    /**
     * \brief バイナリ読み込みのためのユーティリティ
     */
    class BinaryReader
    {
    public:
        /**
         * \brief コンストラクタ
         * \param binary 読み込むを行うバイナリ
         */
        BinaryReader(const Binary& binary);
        
        /**
         * \brief カレントからT型のサイズ分データを読み込み、読み込んだ分カレントを進める
         * \tparam T データの型
         * \return 読み込んだデータ
         */
        template<typename T>
        T Read();

        /**
         * \brief カレントからT型数 * dataCount のサイズ分データを読み込み、読み込んだ分カレントを進める
         * \tparam T データの型
         * \param dataCount データの数
         * \return 読み込んだデータ配列
         */
        template<typename T>
        MemoryManagedArray<T> ReadArray(unsigned dataCount);

        /**
         * \brief 最初に見つかった'\0'までを文字列として読み込み、読み込んだ分カレントを進める
         * \return 読み込んだString
         */
        String ReadString();

        /**
         * \brief カレント位置を移動させます
         * \param offset 移動させるバイナリindex
         * \return 自身が戻り値として返却される
         */
        BinaryReader& Seek(unsigned offset);

        /**
         * \brief カレントからcurrent_to_offsetまでの新しいバイナリリーダを作成する
         * \param current_to_offset 現在位置からのサイズ
         * \return 作成されたバイナリリーダ
         */
        BinaryReader MakeSubReaderFromCurrent(unsigned current_to_offset) const;

        /**
         * \brief 先頭から数えてhead_to_offeset から count までの新しいバイナリリーダを作成する
         * \param head_to_offeset 先頭からの開始インデックス
         * \param count head_to_offesetから読み込むサイズ , 0 の場合はhead_to_offesetから最後まで読み込む
         * \return 作成されたバイナリリーダ
         */
        BinaryReader MakeSubReaderFromHead(unsigned head_to_offeset, unsigned count = 0) const;
    private:
        unsigned _current;
        Binary _binary;
    };
}
#include "BinaryReader.hpp"