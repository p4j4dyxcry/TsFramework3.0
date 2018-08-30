#pragma once
//---------------------------------------------------------------
//! Lruキャッシュ

template<typename TKey, typename TValue>
class LruCache 
{
public:
    using Pair = std::pair<TKey, TValue>;
    using Iterator = typename std::list<Pair>::iterator;

    LruCache(size_t max_size) :_capacity(max_size) {}

    //! 登録
    void Register(const TKey& key, const TValue& value) 
    {
        auto it = _cache.find(key);

        _list.push_front(Pair(key, value));
        if (it != _cache.end()) 
        {
            _list.erase(it->second);
            _cache.erase(it);
        }

        _cache[key] = _list.begin();
        if (_cache.size() > _capacity) 
        {
            auto last = _list.end();
            last--;
            _cache.erase(last->first);
            _list.pop_back();
        }
    }

    //! 取得
    TValue& operator[](const TKey & key)
    {
        return Get(key);
    }

    TValue& Get(const TKey& key) 
    {
        auto it = _cache.find(key);
        if (it == _cache.end()) 
            Error::Assert(false,_T("キャッシュに存在しない"));

        _list.splice(_list.begin(), _list, it->second);
        return it->second->second;
    }

    bool Contains(const TKey& key) const 
    {
        return _cache.find(key) != _cache.end();
    }

    size_t size() const 
    {
        return _cache.size();
    }

private:
    TsList<Pair> _list;
    TsMap<TKey, Iterator> _cache;
    size_t _capacity;
};
