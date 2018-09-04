#pragma once

namespace TS
{
    //! 配列クラス、メモリ管理の責任は負わない
    template<typename T>
    struct Array
    {
    public:
        Array(T* data = nullptr, size_t sz = 0) :_data(data), _size(sz) {}

        operator T*() { return _data; }
        operator const T*() const { return _data; }
    public:
        void Delete()
        {
            if (_data != nullptr)
                delete _data;
            _data = nullptr;
            _size = 0;
        }
    public:
        T * _data;
        size_t _size;
    };

    //! 参照カウンタベースでメモリ解放責務を追う配列クラス
    template<typename T>
    struct SmartArray : Array<T>
    {
    protected:

        class RefCounter
        {
        private:
            unsigned m_referenceCount;
        public:
            RefCounter()
                : m_referenceCount(0) {}
            RefCounter* AddRef() { ++(m_referenceCount); return this; }
            RefCounter* SubRef() { --(m_referenceCount); return this; }
            unsigned GetRef()const { return m_referenceCount; }
        };

        RefCounter * _refarenceConter{};

    public:
        ~SmartArray()
        {
            Release();
        };
        SmartArray(T* data = nullptr, size_t sz = 0, RefCounter* pRefCounter = nullptr) : Array<T>(data, sz)
        {
            _refarenceConter = pRefCounter;

            if (_refarenceConter == nullptr)
                _refarenceConter = new RefCounter();

            if (_refarenceConter != nullptr)
                _refarenceConter->AddRef();
        }
        SmartArray(size_t sz = 0) :SmartArray(new T[sz], sz) {}
        SmartArray(Array<T> array)
        {
            _data = new T[sz];
            _size = sz;
            size_t dataSize = sizeof(T) * sz;
            memcpy_s(_data, dataSize, array._data, dataSize);
            Release();
            _refarenceConter = ref._refarenceConter;
            if (_refarenceConter != nullptr)
                _refarenceConter->AddRef();

        }
        SmartArray(const SmartArray<T>& ref) :SmartArray<T>(ref._data, ref._size, ref._refarenceConter) {}

        SmartArray<T>& operator = (const SmartArray<T>& ref)
        {
            _data = ref._data;
            _size = ref._size;
            Release();
            _refarenceConter = ref._refarenceConter;
            if (_refarenceConter != nullptr)
                _refarenceConter->AddRef();
            return *this;
        }
    protected:
        void Release()
        {
            if (_refarenceConter == nullptr)
                return;
            _refarenceConter->SubRef();
            if (_refarenceConter->GetRef() != 0)
                return;

            Array<T>::Delete();
            delete _refarenceConter;
            _refarenceConter = nullptr;
        }
    };

    class TsString : public SmartArray<TsChar>
    {
    public:
        using SmartArray<TsChar>::SmartArray;
        using SmartArray<TsChar>::operator=;

        TsString(const TsChar * str) :SmartArray<TsChar>(nullptr, 0)
        {
            *this = str;
        }

        TsString& operator = (const TsChar * str)
        {
            while (str[_size++] != '\0') {}

            for (unsigned i = 0; i < _size; ++i)
                _data[i] = str[i];

            Release();
            _refarenceConter = new RefCounter();
            _refarenceConter->AddRef();

            return *this;
        }
    };

    using Binary = SmartArray<unsigned char>;

    class BinaryReader
    {
    public:
        BinaryReader(const Binary& binary) :_binary(binary), _current(0)
        {

        }

        template<typename T>
        T Read()
        {
            T data;
            const size_t sz = sizeof(T);
            memcpy_s(&data, sz, &_binary[_current], sz);
            _current += static_cast<unsigned>(sz);
            return data;
        }

        template<typename T>
        SmartArray<T> ReadArray(unsigned dataCount)
        {
            const size_t sz = sizeof(T) * dataCount;
            SmartArray<T> datas(dataCount);
            memcpy_s(datas, sz, &_binary[_current], sz);
            _current += static_cast<unsigned>(sz);
            return datas;
        }

        TsString ReadString()
        {
            unsigned counter = _current;
            while (counter < _binary._size && _binary[counter++] != '\0') {}

            TsString string(counter);
            for (unsigned i = 0; i < counter; ++i)
                string[i] = _binary[_current++];
            return string;
        }

        BinaryReader& Seek(unsigned offset)
        {
            _current = offset;
            return *this;
        }

        BinaryReader MakeSubReaderFromCurrent(unsigned current_to_offset)
        {
            return { Binary(&_binary[_current], current_to_offset) };
        }

        BinaryReader MakeSubReaderFromZero(unsigned zero_to_offeset, unsigned count = 0)
        {
            if (count == 0)
                count = static_cast<unsigned>(_binary._size) - zero_to_offeset;

            return { Binary(&_binary[zero_to_offeset], count) };
        }

        unsigned _current;
        Binary _binary;
    };
}