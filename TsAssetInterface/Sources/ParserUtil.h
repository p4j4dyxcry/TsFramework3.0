#pragma once
#include "FileReader.h"

inline bool FirstOf(const TS::Binary& binary, const char * tag)
{
    unsigned index = 0;
    while (tag[index] != '\0')
    {
        if (binary[index] != static_cast<unsigned char>(tag[index]))
            return false;
        index++;
    }
    return true;
}


inline void SkipChars(TS::FileReader& reader, const char* elements, unsigned element_count)
{
    const auto equal = [&](char c)
    {
        for (unsigned i = 0; i < element_count; ++i)
        {
            if (c == elements[i])
                return true;
        }
        return false;
    };

    auto data = reader.ToArray();
    unsigned index = reader.Current();
    while (index < data.Length())
    {
        if (equal(data[index++]))
        {
            while (equal(data[index]))
                index++;
            reader.Seek(reader.Current() + index);
            break;
        }
    }
}

inline void SkipChar(TS::FileReader& reader, const char element)
{
    SkipChars(reader, reinterpret_cast<const char*>(element), 1);
}

inline void SeekNextStringEnd(TS::FileReader& reader,const char* tag)
{
    int p = reader.Find(tag);

    if (p == -1)
        return;

    const size_t offset = p + TS::StringA::StringLength(tag);

    if (offset >= reader.ToArray().Length())
        return;
   
    reader.Seek( offset );
}


inline void SkipWhiteSpace(TS::FileReader& reader)
{
    const char chars[] =
    {
        ' ' ,
        '\t',
    };
    SkipChars(reader, chars, ARRAYSIZE(chars));
}

inline int ReadInt(TS::FileReader& source)
{
    int result;
    SkipWhiteSpace(source);
    const char * data = reinterpret_cast<const char *>(source.ToArray().Data());
    sscanf_s(data,
        "%d",
        &result);
    return result;
};

inline float ReadFloat(TS::FileReader& source)
{
    float result;
    SkipWhiteSpace(source);
    const char * data = reinterpret_cast<const char *>(source.ToArray().Data());
    sscanf_s(data,
        "%f",
        &result);
    return result;
};

inline TS::Vector2 ReadVector2(TS::FileReader& source)
{
    TS::Vector2 vector;
    SkipWhiteSpace(source);
    const char * data = reinterpret_cast<const char *>(source.ToArray().Data());
    sscanf_s(data,
        "%f %f",
        &vector.x,
        &vector.y);
    return vector;
};

inline TS::Vector3 ReadVector3(TS::FileReader& source)
{
    TS::Vector3 vector;
    SkipWhiteSpace(source);
    const char * data = reinterpret_cast<const char *>(source.ToArray().Data());
    sscanf_s(data,
        "%f %f %f",
        &vector.x,
        &vector.y,
        &vector.z);
    return vector;
};

inline TS::StringA ReadString(TS::FileReader& reader)
{
    SkipWhiteSpace(reader);
    auto data = reader.ToArray();
    data.Data()[data.Length() - 1] = '\0';
    return reinterpret_cast<const char *>(data.Data());
}

inline TS::StringA ReadStringWithDoubleQuotation(TS::FileReader& reader)
{
    SeekNextStringEnd(reader,"\"");    
    auto data = reader.ToArray();
    data.Data()[reader.Find("\"")] = '\0';
    return reinterpret_cast<const char *>(data.Data());
}

inline TS::Collection<float> ReadFloatArray(TS::FileReader& source, unsigned count)
{
    TS::Collection<float> result;
    result.Resize(count);
    for (unsigned i = 0; i < count; ++i)
    {
        SkipWhiteSpace(source);
        const char * data = reinterpret_cast<const char *>(source.ToArray().Data());
        sscanf_s(data,
            "%f",
            &result[i]);
    }

    return result;
};

inline TS::Collection<int> ReadIntArray(TS::FileReader& source, unsigned count)
{
    TS::Collection<int> result;
    result.Resize(count);
    for (unsigned i = 0; i < count; ++i)
    {
        SkipWhiteSpace(source);
        const char * data = reinterpret_cast<const char *>(source.ToArray().Data());
        sscanf_s(data,
            "%d",
            &result[i]);
    }

    return result;
};

inline TS::Collection<unsigned> ReadUnsignedIntArray(TS::FileReader& source, unsigned count)
{
    TS::Collection<unsigned> result;
    result.Resize(count);
    for (unsigned i = 0; i < count; ++i)
    {
        SkipWhiteSpace(source);
        const char * data = reinterpret_cast<const char *>(source.ToArray().Data());
        sscanf_s(data,
            "%d",
            &result[i]);
    }

    return result;
};

template<typename T>
inline TS::Collection<T> ConvertGeometoryToTriangleList(const T* indexList, const unsigned face_count)
{
    TS::Collection<unsigned> converted_indeces;
    converted_indeces.Reserve(face_count * 3);
    const T* h = &indexList[0];
    const T* m = &indexList[0] + 1;
    for (unsigned i = 0; i < face_count - 2; ++i)
    {
        converted_indeces.Add(*h);
        converted_indeces.Add(*(m++));
        converted_indeces.Add(*m);
    }
    return converted_indeces;
}