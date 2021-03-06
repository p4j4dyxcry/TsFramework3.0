#pragma once

#include "ManagedArray.h"

namespace TS
{
    using Binary = Array<unsigned char>;

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
        Collection<T> ReadArray(unsigned dataCount);

        template<typename T>
        BinaryReader& ReadArray(T* datas, unsigned dataCount);

        /**
         * \brief 最初に見つかった'\0'までを文字列として読み込み、読み込んだ分カレントを進める
         * \return 読み込んだString
         */
        StringA ReadString();

        /**
         * \brief カレント位置を移動させます
         * \param offset 移動させるバイナリindex
         * \return 自身が戻り値として返却される
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