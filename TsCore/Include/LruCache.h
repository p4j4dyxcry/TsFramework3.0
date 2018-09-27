#pragma once

#include "Common.h"
#include <list>
#include <unordered_map>

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
        std::list<std::pair<TKey, TValue>> _list;
        std::unordered_map<TKey, typename std::list<std::pair<TKey, TValue>>::iterator> _dictionary;
        size_t _capacity;
    };
}
#include "LruCache.hpp"
