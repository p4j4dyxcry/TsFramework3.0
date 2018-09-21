#pragma once


namespace TS
{
    /**
    * \brief キャッシュクラス、
    * \tparam TKey
    * \tparam TValue
    */
    template<typename TKey, typename TValue>
    class LruCache
    {
    private:
        using Pair     = std::pair<TKey, TValue>;
        using Iterator = typename std::list<Pair>::iterator;

    public:

        /**
         * \brief コンストラクタ
         * \param capcity キャッシュの容量
         */
        LruCache(size_t capcity);

        
        /**
         * \brief キャッシュに登録
         * \param key 
         * \param value 
         * \result 自身の参照
         */
        LruCache<TKey,TValue>& Register(const TKey& key, const TValue& value);

        /**
         * \brief キャッシュから取得
         * \param key 
         */
        TValue& Get(const TKey& key);

        /**
         * \brief キャッシュにキーが存在するか
         * \param key 
         * \return 存在するならtrue
         */
        bool Contains(const TKey& key) const;

        /**
         * \brief キャッシュの容量を取得
         */
        size_t Size() const;

        TValue& operator[](const TKey& key);

    private:
        TsList<Pair> _list;
        TsMap<TKey, Iterator> _dictionary;
        size_t _capacity;
    };
}
#include "LruCache.hpp"
