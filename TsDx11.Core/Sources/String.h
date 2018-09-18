#pragma once

namespace TS
{

    /**
     * \brief 文字列クラス
     */
    template<typename T>
    class String : public ManagedArray<T>
    {
    public:
        
        /**
         * \brief コンストラクタ
         * \param str 初期文字列
         */
        String(const T* str );

        /**
         * \brief 分割されたサブ文字列の作成
         * \param pos 開始Index
         * \param npos  pos からの カウント , 0の場合は終端までとして扱う
         * \return 分割されたサブ文字列
         */
        String<T> SubString(unsigned pos, unsigned npos = 0) const;

        /**
         * \brief 文字列検索
         * \param _pattern 検索文字列
         * \return 見つかった文字列へのIndex、見つからなかった場合は -1
         */
        int Find(const T* _pattern) const;

        /**
         * \brief 文字置換
         * \param original 置換する文字
         * \param _new 置換される文字 
         * \return 置換された文字列
         */
        String<T> Replace(T original, T _new) const;

        /**
         * \brief 文字列置換
         * \param start 置換する開始インデックス
         * \param size 置換する開始インデックスからのカウント
         * \param str 置換後の文字列 sizeを超える文字列も許容される
         * \return 置換された文字列
         */
        String<T> Replace(unsigned start, unsigned size, const String<T>& str) const;

        /**
         * \brief 文字列置換
         * \param original 置換する文字
         * \param _new 置換される文字
         * \return 置換された文字列
         */
        String<T> Replace(const T* original, const T* _new) const;
        
        /**
         * \brief 対象の文字列が含まれるか調べる
         * \param str 対象の文字列
         * \return 含まれる場合はtrue
         */
        bool Contain(const T* str) const;
               
        /**
         * \brief 文字列が空かnullを判定
         * \return null or 空文字なら true
         */
        bool IsNullOrEmpty() const;
        
        //! Static-------------------------------------------------------------
        /**
        * \brief 文字数を調べる
        * \param str 対象の文字列
        * \return 文字数
        */
        static size_t Length(const T* str);
        
        /**
         * \brief 書式から文字列を作成する
         * \param format 書式
         * \param ... 
         * \return 生成された文字列
         */
        static String<T> Format(const T* format, ...) ;

        // operator
    public:
        using ManagedArray<T>::ManagedArray;

        bool operator ==(const String<T>& string) const;
        bool operator !=(const String<T>& string) const;

        String<T>& operator =(const T* str);
        String<T> operator  +(const T* str) const;
        String<T>& operator +=(const T* str);

        String(const String<T>& ref);
        String(const String<T>&& ref) noexcept;

        String<T>& operator =(const String<T>& ref);
        String<T>& operator =(String<T>&& ref) noexcept;
        
    };

    using StringA = String<char>;
    using StringW = String<wchar_t>;
}
#include "String.hpp"