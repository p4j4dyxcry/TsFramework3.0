#pragma once

namespace TS
{

    /**
     * \brief 文字列クラス、メモリ管理は参照カウンタ式になっているので不要
     */
    class TsString : public MemoryManagedArray<TsChar>
    {
    public:
        
        /**
         * \brief コンストラクタ
         * \param str 初期文字列
         */
        TsString(const TsChar* str = _T(""));

        /**
         * \brief 分割されたサブ文字列の作成
         * \param pos 開始Index
         * \param npos  pos からの カウント , 0の場合は終端までとして扱う
         * \return 分割されたサブ文字列
         */
        TsString SubString(unsigned pos, unsigned npos = 0) const;

        /**
         * \brief 文字列検索
         * \param _pattern 検索文字列
         * \return 見つかった文字列へのIndex、見つからなかった場合は -1
         */
        int Find(const TsChar* _pattern) const;

        /**
         * \brief 文字置換
         * \param original 置換する文字
         * \param _new 置換される文字 
         * \return 置換された文字列
         */
        TsString Replace(TsChar original, TsChar _new) const;

        /**
         * \brief 文字列置換
         * \param start 置換する開始インデックス
         * \param size 置換する開始インデックスからのカウント
         * \param str 置換後の文字列 sizeを超える文字列も許容される
         * \return 置換された文字列
         */
        TsString Replace(unsigned start, unsigned size, const TsString& str) const;

        /**
         * \brief 文字列置換
         * \param original 置換する文字
         * \param _new 置換される文字
         * \return 置換された文字列
         */
        TsString Replace(const TsChar* original, const TsChar* _new) const;
        
        /**
         * \brief 対象の文字列が含まれるか調べる
         * \param str 対象の文字列
         * \return 含まれる場合はtrue
         */
        bool Contain(const TsChar* str) const;
               
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
        static size_t Length(const TsChar* str);
        
        /**
         * \brief 書式から文字列を作成する
         * \param format 書式
         * \param ... 
         * \return 生成された文字列
         */
        static TsString Format(const TsChar* format, ...) ;

        // operator
    public:
        using MemoryManagedArray<TsChar>::MemoryManagedArray;
        using MemoryManagedArray<TsChar>::operator=;
        bool operator ==(const TsString& string) const;
        bool operator !=(const TsString& string) const;
        TsString& operator =(const TsChar* str);
        TsString operator  +(const TsChar* str) const;
        TsString& operator +=(const TsChar* str);
    };
}

