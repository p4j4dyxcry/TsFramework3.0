#include "pch.h"
#include "TsDx11Core.Internal.h"

namespace TS
{
    class BinaryReader
    {
    public:
        BinaryReader(Binary binary) :_binary(binary), _current(0)
        {

        }

        template<typename T>
        T ReadDeta()
        {
            T data;
            size_t sz = sizeof(T);
            memcpy_s(&data, sz, &_binary[_current], sz);
            _current += sz;
            return data;
        }

        template<typename T>
        T* ReadDetaArray(T* datas, unsigned dataCount)
        {
            size_t sz = sizeof(T) * dataCount;
            memcpy_s(datas, sz, &_binary[_current], sz);
            _current += sz;
            return datas;
        }

        void ReadString(char * ptr, unsigned bufferSize)
        {
            for (unsigned i = 0; i < bufferSize; ++i)
            {
                if (_current + i >= _binary._size)
                    return;

                ptr[i] = _binary[_current++];

                if (ptr[i] == 0)
                    return;
            }
        }

        BinaryReader& Seek(unsigned offset)
        {
            _current = offset;
            return *this;
        }

        BinaryReader SubBinaryReader(unsigned offset)
        {
            return { Binary(&_binary[_current], offset) };
        }

        BinaryReader SeekBinaryReader(unsigned offset, unsigned count = 0)
        {
            if (count == 0)
                count = _binary._size - offset;

            return { Binary(&_binary[offset], count) };
        }

        unsigned _current;
        Binary _binary;
    };

    enum FormatType
    {
        UNKNOWN = 0,
        UINT32 = 1,
        SINT32 = 2,
        FLOAT32 = 3,
        UNORM = 4,
    };

    DXGI_FORMAT ConvertFormat(unsigned dataCount, unsigned type)
    {
        if (type == FLOAT32)
        {
            if (dataCount == 1) return DXGI_FORMAT_R32_FLOAT;
            if (dataCount == 2) return DXGI_FORMAT_R32G32_FLOAT;
            if (dataCount == 3) return DXGI_FORMAT_R32G32B32_FLOAT;
            if (dataCount == 4) return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        if (type == UINT32)
        {
            if (dataCount == 1) return DXGI_FORMAT_R32_UINT;
            if (dataCount == 2) return DXGI_FORMAT_R32G32_UINT;
            if (dataCount == 3) return DXGI_FORMAT_R32G32B32_UINT;
            if (dataCount == 4) return DXGI_FORMAT_R32G32B32A32_UINT;
        }
        if (type == SINT32)
        {
            if (dataCount == 1) return DXGI_FORMAT_R32_SINT;
            if (dataCount == 2) return DXGI_FORMAT_R32G32_SINT;
            if (dataCount == 3) return DXGI_FORMAT_R32G32B32_SINT;
            if (dataCount == 4) return DXGI_FORMAT_R32G32B32A32_SINT;
        }

        if (type == UNORM)
        {
            if (dataCount == 1) return DXGI_FORMAT_R8_UNORM;
            if (dataCount == 2) return DXGI_FORMAT_R8G8_UNORM;
            if (dataCount == 3) return DXGI_FORMAT_UNKNOWN;
            if (dataCount == 4) return DXGI_FORMAT_R8G8B8A8_UNORM;
        }

        return DXGI_FORMAT_UNKNOWN;
    }

    InputElementDesc MakeElementDescFloat(const char* name, unsigned dataCount, unsigned semanticIndex)
    {
        return{
            name
            , semanticIndex
            , ConvertFormat(dataCount,FLOAT32)
            , 0
            , D3D11_APPEND_ALIGNED_ELEMENT
            , D3D11_INPUT_PER_VERTEX_DATA
            , 0
        };
    }

    InputElementDesc MakeElementDescUInt(const char* name, unsigned dataCount, unsigned semanticIndex)
    {
        return{
            name
            , semanticIndex
            , ConvertFormat(dataCount,UINT32)
            , 0
            , D3D11_APPEND_ALIGNED_ELEMENT
            , D3D11_INPUT_PER_VERTEX_DATA
            , 0
        };
    }

    InputElementDesc MakeElementDescUnorm(const char* name, unsigned dataCount, unsigned semanticIndex)
    {
        return{
            name
            , semanticIndex
            , ConvertFormat(dataCount,UNORM)
            , 0
            , D3D11_APPEND_ALIGNED_ELEMENT
            , D3D11_INPUT_PER_VERTEX_DATA
            , 0
        };
    }

    InputElementDesc MakeElementDescSint(const char* name, unsigned dataCount, unsigned semanticIndex)
    {
        return{
            name
            , semanticIndex
            , ConvertFormat(dataCount,SINT32)
            , 0
            , D3D11_APPEND_ALIGNED_ELEMENT
            , D3D11_INPUT_PER_VERTEX_DATA
            , 0
        };
    }

    InputElementDesc ElementDescPosition3f(unsigned id)
    {
        return MakeElementDescFloat("POSITION", 3, id);
    }

    InputElementDesc ElementDescPosition4f(unsigned id)
    {
        return MakeElementDescFloat("POSITION", 4, id);
    }

    InputElementDesc ElementDescUv2f(unsigned id)
    {
        return MakeElementDescFloat("TEXCOORD", 2, id);
    }

    InputElementDesc ElementDescColor3f(unsigned id)
    {
        return MakeElementDescFloat("COLOR", 3, id);
    }

    InputElementDesc ElementDescColor4f(unsigned id)
    {
        return MakeElementDescFloat("COLOR", 4, id);
    }

    InputElementDesc ElementDescColor4u(unsigned id)
    {
        return MakeElementDescUnorm("COLOR", 4, id);
    }

    InputElementDesc ElementDescNormal3f(unsigned id)
    {
        return MakeElementDescFloat("NORMAL", 3, id);
    }

    InputElementDesc ElementDescNormal4f(unsigned id)
    {
        return MakeElementDescFloat("NORMAL", 4, id);
    }

    InputElementDesc ElementDescTangent4f(unsigned id)
    {
        return MakeElementDescFloat("TANGENT", 4, id);
    }

    InputElementDesc ElementDescBinormal4f(unsigned id)
    {
        return MakeElementDescFloat("BINORMAL", 4, id);
    }

    InputElementDesc ElementDescBlendIndices4u(unsigned id)
    {
        return MakeElementDescFloat("BLENDINDICES", 4, id);
    }

    InputElementDesc ElementDescBlendWeightf(unsigned id)
    {
        return MakeElementDescFloat("BLENDWEIGHT", 4, id);
    }

    InputElementDesc ElementDescVertexId()
    {
        return MakeElementDescFloat("SV_VertexID", 1, 0);
    }

    InputElementDesc ElementDescInstanceId()
    {
        return MakeElementDescFloat("SV_InstanceID", 1, 0);
    }

    InputElementDesc ElementDescPrimitiveId()
    {
        return MakeElementDescUInt("SV_PrimitiveID", 1, 0);
    }

    unsigned Join3Byte(unsigned char b0, unsigned char b1, unsigned char b2)
    {
        return b0 * 256 * 256 + b1 * 256 + b2;
    }

    enum ChunkType
    {
        Ifce,
        Isgn,
        Osgn,
        Osg5,
        Pcsg,
        Rdef,
        Sdbg,
        Sfi0,
        Shdr,
        Shex,
        Spdb,
        Stat,
        Unknown
    };

    ChunkType GetChunkType(char cc[4])
    {
        const char* _knowTypes[] =
        {
            "IFCE",
            "ISGN",
            "OSGN",
            "OSG5",
            "PCSG",
            "RDEF",
            "SDBG",
            "SFI0",
            "SHDR",
            "SHEX",
            "SPDB",
            "STAT",
        };
        for (int i = 0; i < ARRAYSIZE(_knowTypes); ++i)
        {
            if (memcmp(cc, _knowTypes[i], 4) == 0)
            {
                printf("%d:%s\n", i, _knowTypes[i]);
                return static_cast<ChunkType>(i);
            }
        }
        return Unknown;
    }

    struct HlslBinaryHeader
    {
        unsigned char FourCC[4];
        unsigned char Guid[16];
        unsigned One;
        unsigned Size;
        unsigned ChunkCount;
    };

    unsigned GenerateMask(unsigned char s, unsigned char e)
    {
        unsigned mask = 0;
        for (; s < e; ++s)
        {
            unsigned flags = 2;
            for (int i = 0; i < s; ++i)
                flags *= 2;
            mask |= flags;
        }
        return mask;
    }

    template<typename T>
    T DecodeValue(unsigned token, unsigned start, unsigned end)
    {
        const unsigned mask = GenerateMask(start, end);
        const unsigned shift = start;

        return static_cast<T>(token & mask) >> shift;
    }

    struct ShaderVersion
    {
        unsigned Major;
        unsigned Minor;
        ShaderType Type;
    };

    static ShaderType TokenToType(unsigned shader_type_token)
    {
        switch (shader_type_token)
        {
        case 0xffff: return Ts_Pixel;
        case 0xfffe: return Ts_Vertex;
        case 0x4853: return Ts_Hull;
        case 0x4753: return Ts_Geometry;
        case 0x4453: return Ts_Domain;
        case 0x4353: return Ts_Compute;
        default:     return ShaderType_Unknown;
        }
    }

    static ShaderVersion GetShaderVersion(unsigned version_token)
    {
        ShaderVersion shader_version;
        shader_version.Minor = DecodeValue<unsigned>(version_token, 0, 7);
        shader_version.Major = DecodeValue<unsigned>(version_token, 8, 15);
        const unsigned shader_type_token = DecodeValue<unsigned>(version_token, 16, 31);
        shader_version.Type = TokenToType(shader_type_token);

        return shader_version;
    }

    enum ShaderCompileOptionFlags
    {
        None = 0,
        Debug = 1,
        SkipValidation = 2,
        SkipOptimization = 4,
        PackMatrixRowMajor = 8,
        PackMatrixColumnMajor = 16,
        PartialPrecision = 32,
        ForceVsSoftwareNoOpt = 64,
        ForcePsSoftwareNoOpt = 128,
        NoPreshader = 256,
        AvoidFlowControl = 512,
        PreferFlowControl = 1024,
        EnableStrictness = 2048,
        EnableBackwardsCompatibility = 4096,
        IeeeStrictness = 8192,
        OptimizationLevel0 = 16384,
        OptimizationLevel1 = 0,
        OptimizationLevel2 = 49152,
        OptimizationLevel3 = 32768,
        Reserved16 = 65536,
        Reserved17 = 131072,
        WarningsAreErrors = 262144
    };

    enum class ShaderVariableFlags
    {
        None = 0,
        UserPacked = 1,
        Used = 2,
        InterfacePointer = 4,
        InterfaceParameter = 8
    };

    enum class ShaderInputType
    {
        CBuffer = 0,
        TBuffer = 1,
        Texture = 2,
        Sampler = 3,
        UavRwTyped = 4,
        Structured = 5,
        UavRwStructured = 6,
        ByteAddress = 7,
        UavRwByteAddress = 8,
        UavAppendStructured = 9,
        UavConsumeStructured = 10,
        UavRwStructuredWithCounter = 11
    };

    enum class ShaderInputFlags
    {
        None,
        UserPacked = 0x1,
        ComparisonSampler = 0x2,
        TextureComponent0 = 0x4,
        TextureComponent1 = 0x8,
        TextureComponents = 0xc,
        Unused = 0x10
    };

    enum class ResourceReturnType
    {
        NotApplicable = 0,
        UNorm = 1,
        SNorm = 2,
        SInt = 3,
        UInt = 4,
        Float = 5,
        Mixed = 6,
        Double = 7,
        Continued = 8
    };

    enum class ShaderResourceViewDimension
    {
        Unknown = 0,
        Buffer = 1,
        Texture1D = 2,
        Texture1DArray = 3,
        Texture2D = 4,
        Texture2DArray = 5,
        Texture2DMultiSampled = 6,
        Texture2DMultiSampledArray = 7, Texture3D = 8,
        TextureCube = 9,
        TextureCubeArray = 10,
        ExtendedBuffer = 11
    };

    struct RdefHeader
    {
        unsigned CBufferCount{};
        unsigned CBufferOffset{};
        unsigned ResourceBindingCount{};
        unsigned ResourceBindingOffset{};
        unsigned Version{};
        ShaderCompileOptionFlags Flags{};
    };

    struct TypeMember;

    struct DataType
    {
        D3D_SHADER_VARIABLE_CLASS VariableClass{};
        D3D_SHADER_VARIABLE_TYPE  VariableType{};

        char Name[128]{};

        unsigned short Rows{};
        unsigned short Columns{};
        unsigned short ElementCount{};
        Array<TypeMember> Members{};

        void Delete()
        {
            Members.Delete();
        }
    };

    struct TypeMember
    {
        char Name[128]{};
        unsigned Offset{};
        DataType Type{};
    };

    struct ShaderVariable
    {
        char Name[128]{};

        unsigned StartTexture{};
        unsigned TextureSize{};
        unsigned StartSampler{};
        unsigned SamplerSize{};

        unsigned Size{};
        ShaderVariableFlags Flags{};
        TypeMember Member{};

        Array<unsigned> DefaultValues{};

        void Delete()
        {
            DefaultValues.Delete();
        }
    };

    struct ConstantBufferData
    {
        char Name[128]{};
        D3D_CBUFFER_TYPE Type{};
        unsigned Size{};
        D3D_CBUFFER_TYPE  BufferType{};
        D3D_SHADER_CBUFFER_FLAGS Flags{};
        Array<ShaderVariable> Variables{};

        void Delete()
        {
            Variables.Delete();
        }
    };

    struct ResourceBindingData
    {
        char Name[128]{};
        ShaderInputType Type{};
        unsigned BindPoint{};
        ShaderInputFlags Flags{};
        ResourceReturnType ReturnType{};
        unsigned NumSamples{};
        unsigned BindCount{};
        ShaderResourceViewDimension Dimension;

    };

    struct RdefChunk
    {
        RdefHeader Header{};
        ShaderVersion TargetVersion{};
        Array<ConstantBufferData> CBufferData{};
        Array<ResourceBindingData> ResourceBindingData{};

        void Delete()
        {
            CBufferData.Delete();
            ResourceBindingData.Delete();
        }
    };
    DataType ReadDataType(BinaryReader& reader, BinaryReader& typeReader, ShaderVersion targetVersion);

    TypeMember ReadTypeMember(BinaryReader& reader, BinaryReader& memberReader, ShaderVersion targetVersion)
    {
        TypeMember result;

        unsigned nameOffset = memberReader.ReadDeta<unsigned>();
        BinaryReader nameReader = reader.SeekBinaryReader(nameOffset);
        nameReader.ReadString(result.Name, ARRAYSIZE(result.Name));

        unsigned memberTypeOffset = memberReader.ReadDeta<unsigned>();

        result.Offset = memberReader.ReadDeta<unsigned>();

        BinaryReader memberTypeReader = reader.SeekBinaryReader(memberTypeOffset);
        result.Type = ReadDataType(reader, memberTypeReader, targetVersion);
        return result;
    }

    DataType ReadDataType(BinaryReader& reader, BinaryReader& typeReader, ShaderVersion targetVersion)
    {
        DataType result;
        result.VariableClass = static_cast<D3D_SHADER_VARIABLE_CLASS>(typeReader.ReadDeta<unsigned short>());
        result.VariableType = static_cast<D3D_SHADER_VARIABLE_TYPE>(typeReader.ReadDeta<unsigned short>());
        result.Rows = typeReader.ReadDeta<unsigned short>();
        result.Columns = typeReader.ReadDeta<unsigned short>();
        result.ElementCount = typeReader.ReadDeta<unsigned short>();

        unsigned short memberCount = typeReader.ReadDeta<unsigned short>();
        unsigned memberOffset = typeReader.ReadDeta<unsigned>();

        if (targetVersion.Major >= 5)
        {
            unsigned parentTypeOffset = typeReader.ReadDeta<unsigned>(); // Guessing
            if (parentTypeOffset != 0)
            {
                BinaryReader parentTypeReader = reader.SeekBinaryReader(parentTypeOffset);
                D3D_SHADER_VARIABLE_CLASS parentTypeClass = static_cast<D3D_SHADER_VARIABLE_CLASS>(parentTypeReader.ReadDeta<unsigned short>());
                unsigned short unknown1 = parentTypeReader.ReadDeta<unsigned>();
            }

            unsigned unknown2 = typeReader.ReadDeta<unsigned>();
            if (unknown2 != 0)
            {
                BinaryReader unknownReader = reader.SeekBinaryReader(unknown2);
                unsigned unknown3 = unknownReader.ReadDeta<unsigned>();
            }

            unsigned unknown4 = typeReader.ReadDeta<unsigned>();

            unsigned unknown5 = typeReader.ReadDeta<unsigned>();
            if (unknown5 != 0)
            {
                BinaryReader unknownReader = reader.SeekBinaryReader(unknown5);
                unsigned unknown6 = unknownReader.ReadDeta<unsigned>();
            }

            unsigned parentNameOffset = typeReader.ReadDeta<unsigned>();
            if (parentNameOffset > 0)
            {
                BinaryReader parentNameReader = reader.SeekBinaryReader(parentNameOffset);
                parentNameReader.ReadString(result.Name, ARRAYSIZE(result.Name));
            }
        }

        if (memberCount > 0)
        {
            result.Members = Array<TypeMember>(new TypeMember[memberCount], memberCount);
            BinaryReader memberReader = reader.SeekBinaryReader(memberOffset);
            for (int i = 0; i < memberCount; i++)
            {
                result.Members[i] = ReadTypeMember(reader, memberReader, targetVersion);
            }
        }
        return result;
    }

    ShaderVariable ReadShaderVariable(BinaryReader& reader, BinaryReader& variableReader, ShaderVersion target)
    {
        unsigned nameOffset = variableReader.ReadDeta<unsigned>();
        BinaryReader nameReader = reader.SeekBinaryReader(nameOffset);

        unsigned startOffset = variableReader.ReadDeta<unsigned>();
        unsigned size = variableReader.ReadDeta<unsigned>();
        ShaderVariableFlags flags = variableReader.ReadDeta<ShaderVariableFlags>();

        unsigned typeOffset = variableReader.ReadDeta<unsigned>();
        BinaryReader typeReader = reader.SeekBinaryReader(typeOffset);
        DataType shaderType = ReadDataType(reader, typeReader, target);

        const unsigned defaultValueOffset = variableReader.ReadDeta<unsigned>();

        ShaderVariable result;
        result.Size = size;
        result.Flags = flags;
        if (defaultValueOffset != 0)
        {
            BinaryReader defaultValueReader = reader.SeekBinaryReader(defaultValueOffset);
            if (size % 4 != 0)
            {
                size = 0;
            }

            if (size != 0)
            {
                size /= 4;
                result.DefaultValues = Array<unsigned>(new unsigned[size], size);
                for (unsigned i = 0; i < size; ++i)
                    result.DefaultValues[i] = defaultValueReader.ReadDeta<unsigned>();
            }
        }

        nameReader.ReadString(result.Name, ARRAYSIZE(result.Name));
        nameReader.Seek(0);
        nameReader.ReadString(result.Member.Name, ARRAYSIZE(result.Member.Name));

        if (target.Major >= 5)
        {
            result.StartTexture = variableReader.ReadDeta<unsigned>();
            result.TextureSize = variableReader.ReadDeta<unsigned>();
            result.StartSampler = variableReader.ReadDeta<unsigned>();
            result.SamplerSize = variableReader.ReadDeta<unsigned>();
        }
        return result;
    }

    ConstantBufferData ReadConstantBuffer(BinaryReader& reader, BinaryReader& cBufferReader, ShaderVersion targetVersion)
    {
        const unsigned nameOffset = cBufferReader.ReadDeta<unsigned>();
        BinaryReader nameReader = reader.SeekBinaryReader(nameOffset);

        const unsigned variableCount = cBufferReader.ReadDeta<unsigned>();
        const unsigned variableOffset = cBufferReader.ReadDeta<unsigned>();

        ConstantBufferData result;
        nameReader.ReadString(result.Name, ARRAYSIZE(result.Name));

        if (variableCount > 0)
        {
            result.Variables = Array<ShaderVariable>(new ShaderVariable[variableCount], variableCount);
            BinaryReader variableReader = reader.SeekBinaryReader(variableOffset);

            for (unsigned i = 0; i < variableCount; i++)
            {
                result.Variables[i] = ReadShaderVariable(reader, variableReader, targetVersion);
                printf(result.Variables[i].Name);
            }
        }
        result.Size = cBufferReader.ReadDeta<unsigned>();
        result.Flags = cBufferReader.ReadDeta<D3D_SHADER_CBUFFER_FLAGS>();
        result.BufferType = cBufferReader.ReadDeta<D3D_CBUFFER_TYPE>();

        return result;
    }

    ResourceBindingData ReadResourceData(BinaryReader& reader, BinaryReader& resourceBindingReader)
    {
        ResourceBindingData result;
        unsigned nameOffset = resourceBindingReader.ReadDeta<unsigned>();
        BinaryReader nameReader = reader.SeekBinaryReader(nameOffset);
        nameReader.ReadString(result.Name, ARRAYSIZE(result.Name));
        result.Type = resourceBindingReader.ReadDeta<ShaderInputType>();
        result.ReturnType = resourceBindingReader.ReadDeta<ResourceReturnType>();
        result.Dimension = resourceBindingReader.ReadDeta<ShaderResourceViewDimension>();
        result.NumSamples = resourceBindingReader.ReadDeta<unsigned>();
        result.BindPoint = resourceBindingReader.ReadDeta<unsigned>();
        result.BindCount = resourceBindingReader.ReadDeta<unsigned>();
        result.Flags = resourceBindingReader.ReadDeta<ShaderInputFlags>();
        return result;
    }

    RdefChunk ReadRdef(BinaryReader& reader)
    {
        RdefChunk result;
        result.Header = reader.ReadDeta<RdefHeader>();
        result.TargetVersion = GetShaderVersion(result.Header.Version);

        BinaryReader constantBufferReader = reader.SeekBinaryReader(result.Header.CBufferOffset);

        if (result.Header.CBufferCount > 0)
        {
            result.CBufferData = Array<ConstantBufferData>(new ConstantBufferData[result.Header.CBufferCount], result.Header.CBufferCount);
            for (unsigned i = 0; i < result.Header.CBufferCount; ++i)
            {
                result.CBufferData[i] = ReadConstantBuffer(reader, constantBufferReader, result.TargetVersion);
            }
        }

        if (result.Header.ResourceBindingCount > 0)
        {
            result.ResourceBindingData = Array<ResourceBindingData>(new ResourceBindingData[result.Header.ResourceBindingCount], result.Header.ResourceBindingCount);
            BinaryReader resourceBindingReader = reader.SeekBinaryReader(result.Header.ResourceBindingOffset);

            for (unsigned i = 0; i < result.Header.ResourceBindingCount; ++i)
            {
                result.ResourceBindingData[i] = ReadResourceData(reader, resourceBindingReader);
            }
        }

        return result;
    }

    void Test(Binary& binary)
    {
        BinaryReader reader(binary);
        const HlslBinaryHeader header = reader.ReadDeta<HlslBinaryHeader>();

        for (unsigned i = 0; i < header.ChunkCount; ++i)
        {
            const unsigned chunkOffset = reader.ReadDeta<unsigned>();
            auto chukReader = reader.SeekBinaryReader(chunkOffset);

            char cc[4];
            chukReader.ReadDetaArray<char>(cc, 4);
            const ChunkType type = GetChunkType(cc);

            if (type == Unknown)
                continue;

            unsigned chunkSize = chukReader.ReadDeta<unsigned>();

            if (type == Rdef)
            {
                BinaryReader reader = chukReader.SubBinaryReader(chunkSize);
                ReadRdef(reader);
            }
        }
    }

    Array<InputElementDesc> MakeInputLayoutDescFromMemory(Binary& binary)
    {
        Test(binary);

        unsigned char *pInputElementEntry = nullptr;
        for (size_t i = 0L; i < binary._size - 4; ++i)
        {
            if (memcmp(&binary[i], "ISGN", 4) == 0)
            {
                pInputElementEntry = &binary[i];
                break;
            }
        }
        if (pInputElementEntry == nullptr)
        {
            return { nullptr,0 };
        }

        const auto ContainSystemSemantices = [](char* _name)
        {
            static const char* SystemSemantices[]
            {
                "SV_InstanceID",
                "SV_VertexID",
                "SV_PrimitiveID",
            };
            for (auto& s : SystemSemantices)
            {
                if (strcmp(_name, s) == 0)
                    return true;
            }
            return false;
        };

        int cntvariable = pInputElementEntry[8];
        int systemSemantices = 0;
        Array<char*> names(new char*[cntvariable], cntvariable);
        Array<unsigned> index(new unsigned[cntvariable], cntvariable);
        Array<DXGI_FORMAT> format(new DXGI_FORMAT[cntvariable], cntvariable);
        unsigned char *str = &pInputElementEntry[16];
        for (int i = 0; i < cntvariable; i++)
        {
            names[i] = reinterpret_cast<char*>(str[i * 24] + pInputElementEntry + 8);

            if (ContainSystemSemantices(names[i]))
            {
                systemSemantices++;
                continue;
            }

            index[i] = str[i * 24 + 4];
            const unsigned bitMask = str[i * 24 + 20];
            const unsigned type = str[i * 24 + 12];

            unsigned dataCount = 0;

            if (bitMask & 0x8) dataCount++;
            if (bitMask & 0x4) dataCount++;
            if (bitMask & 0x2) dataCount++;
            if (bitMask & 0x1) dataCount++;

            format[i] = ConvertFormat(dataCount, type);
        }
        cntvariable -= systemSemantices;

        Array<TS::InputElementDesc> descs(new TS::InputElementDesc[cntvariable], cntvariable);
        for (int i = 0; i < cntvariable; i++)
        {
            descs[i] = {
                names[i],
                index[i],
                format[i],
                0,
                D3D11_APPEND_ALIGNED_ELEMENT,
                D3D11_INPUT_PER_VERTEX_DATA,
                0
            };
        }
        index.Delete();
        names.Delete();
        format.Delete();

        return descs;
    }


}