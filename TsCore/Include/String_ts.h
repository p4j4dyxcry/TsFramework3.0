#pragma once

#include "Common.h"

namespace TS
{

    /**
     * \brief 文字列クラス
     */
    template<typename T>
    class String
    {
    public:
        /**
         * \brief コンストラクタ
         * \param str 初期文字列
         */
        String(const T* str);

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
         * \brief 文字検索
         * \param _pattern 検索文字列
         * \return 見つかった文字列へのIndex、見つからなかった場合は -1
         */
        int Find(const T& _pattern) const;

        /**
         * \brief 文字列検索
         * \param _pattern 検索文字列
         * \return 見つかった文字列への最後のIndex、見つからなかった場合は -1
         */
        int Rfind(const T* _pattern) const;

        /**
         * \brief 文字検索
         * \param _pattern 検索文字列
         * \return 見つかった文字列へのIndex、見つからなかった場合は -1
         */
        int Rfind(const T& _pattern) const;

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
        String<T> Replace(unsigned start, unsigned size, const T* str) const;

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
        static size_t StringLength(const T* str);

        /**
         * \brief 書式から文字列を作成する
         * \param format 書式
         * \param ...
         * \return 生成された文字列
         */
        static String<T> Format(const T* format, ...);

        /**
        * \brief メモリを確保する、確保済み領域が十分な場合は何もしない
        * \param capacity メモリ確保量
        */
        String<T>& Reserve(size_t capacity);

        /**
         * \brief 要素をクリアする
         */
        String<T>& Clear();

        /**
         * \brief 要素数を変更する
         * \param size 
         * \return 
         */
        String<T>& Resize(size_t size);

        /**
         * \brief 文字列を結合する
         */
        String<T>& AddRange(const T* string);

        /**
         * \brief 文字列を取得する
         */
        T* & Data() { return _data; }
        const T* Data()const { return _data; }


        /**
         * \brief 文字列の長さを取得する(終端文字含む)
         */
        size_t Length()const { return _size; }

        //! operator
    public:

        virtual ~ String();

        bool operator ==(const String<T>& string) const;
        bool operator !=(const String<T>& string) const;

        bool operator ==(const T* string) const;
        bool operator !=(const T* string) const;

        String<T>& operator =(const T* str);
        String<T> operator  +(const T* str) const;
        String<T>& operator +=(const T* str);

        String() :_capacity(0), _size(0), _data(nullptr){}
        String(const String<T>& ref);
        String(const String<T>&& ref) noexcept;

        String<T>& operator =(const String<T>& ref);
        String<T>& operator =(String<T>&& ref) noexcept;

        T& operator[](unsigned index);
        const T& operator[](unsigned index) const;

        operator const T*() const { return _data; }

    private:
        String<T>& copy_internal(size_t start, size_t count, const T* buffer);
        String<T>& copy_all_internal(const T* data, size_t size);
    private:
        size_t _capacity;
        size_t _size;
        T*   _data;
    };


    template<typename T>
    T* begin(String<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    T* end(String<T>& s)
    {
        return { s.Data() + s.Length() };
    };

    template<typename T>
    T* begin(const String<T>& s)
    {
        return { s.Data() };
    };

    template<typename T>
    T* end(const String<T>& s)
    {
        return { s.Data() + s.Length() };
    };

    using StringA = String<char>;
    using StringW = String<wchar_t>;
}

#include "String_ts.hpp"