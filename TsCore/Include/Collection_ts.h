#pragma once
#include "Common.h"
namespace TS
{
    /**
     * \brief コレクションクラス
     */
    template<typename T>
    class Collection
    {
    public:

        /**
        * \brief コンストラクタ
        * \param capacity　メモリ確保量
        */
        Collection(size_t capacity);

        /**
        * \brief サイズを変更　確保済みメモリは解放されない
        *        ※新しく生成されたインスタンスはコンストラクタが呼ばれてないので注意
        * \param size　新しいコレクションのサイズ
        */
        Collection<T>& Resize(size_t size);


        /**
         * \brief 要素を全て削除する
         */
        Collection<T>& Clear();


        /**
        * \brief 要素が空かどうか
        */
        bool IsEmpty() const;

        /**
         * \brief メモリを確保する、確保済み領域が十分な場合は何もしない
         * \param capacity メモリ確保量
         */
        Collection<T>& Reserve(size_t capacity);

        /**
         * \brief 要素を追加する
         */
        Collection<T>& Add(const T& value);


        /**
         * \brief コレクション内の２つの要素を入れ替える
         *        呼び出しはコピーコンストラクタ呼び出しが行われる
         * \param index1 
         * \param index2 
         */
        Collection<T>& Swap(size_t index1, size_t index2);

        /**
         * \brief 要素を指定インデックスに挿入する
         */
        Collection<T>& Insert(size_t index, const T& value);

        /**
         * \brief 初期化子を用いて要素を追加する
         */
        Collection<T>& AddRange(const std::initializer_list<T>& items);

        /**
         * \brief 要素を追加する
         */
        Collection<T>& AddRange(const Collection<T>& items);

        /**
         * \brief 最初に見つかった要素を削除する
         */
        Collection<T>& Remove(const T& value);

        /**
         * \brief インデックスで要素を削除する
         */
        Collection<T>& RemoveAt(size_t index);

        /**
         * \brief 指定範囲の要素を削除する
         */
        Collection<T>& RemoveRange(size_t start, size_t count);

        /**
         * \brief 要素数を取得する
         */
        size_t Length() const;


        /**
         * \brief 配列の先頭ポインタを取得する
         */
        T*& Data();
        const T*& Data() const;

    public:
        Collection();

        Collection(const Collection<T>& c);

        Collection(const std::initializer_list<T>& c);

        Collection(Collection<T>&& c) noexcept;

        Collection<T>& operator =(const Collection<T>& c);

        Collection<T>& operator =(Collection<T>&& c) noexcept;

        virtual ~Collection();

        T& operator[](size_t index);

        const T& operator[](size_t index) const;
    private:

        //! 前詰め
        void pack_front(size_t start, size_t end);

        //! 後ろ詰め
        void pack_back(size_t start, size_t end);

        void copy_data(size_t start , size_t count , T* buffer);

        size_t _capacity;
        size_t _size;
        T*   _data;
    };
}
#include "Collection_ts.hpp"