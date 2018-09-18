#pragma once

namespace TS
{
    template <typename TKey, typename TValue>
    LruCache<TKey, TValue>::LruCache(size_t max_size) : _capacity(max_size)
    {
    }

    template <typename TKey, typename TValue>
    LruCache<TKey, TValue>& LruCache<TKey, TValue>::Register(const TKey& key, const TValue& value)
    {
        auto it = _dictionary.find(key);

        _list.push_front(Pair(key, value));
        if (it != _dictionary.end())
        {
            _list.erase(it->second);
            _dictionary.erase(it);
        }

        _dictionary[key] = _list.begin();
        if (_dictionary.size() > _capacity)
        {
            auto last = _list.end();
            last--;
            _dictionary.erase(last->first);
            _list.pop_back();
        }
        return *this;
    }

    template <typename TKey, typename TValue>
    TValue& LruCache<TKey, TValue>::operator[](const TKey& key)
    {
        return Get(key);
    }

    template <typename TKey, typename TValue>
    TValue& LruCache<TKey, TValue>::Get(const TKey& key)
    {
        auto it = _dictionary.find(key);
        if (it == _dictionary.end())
            Error::Assert(false, _T("ƒLƒƒƒbƒVƒ…‚É‘¶Ý‚µ‚È‚¢"));

        _list.splice(_list.begin(), _list, it->second);
        return it->second->second;
    }

    template <typename TKey, typename TValue>
    bool LruCache<TKey, TValue>::Contains(const TKey& key) const
    {
        return _dictionary.find(key) != _dictionary.end();
    }

    template <typename TKey, typename TValue>
    size_t LruCache<TKey, TValue>::Size() const
    {
        return _dictionary.Size();
    }
}