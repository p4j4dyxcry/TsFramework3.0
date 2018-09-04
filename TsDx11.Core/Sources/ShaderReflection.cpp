#include "pch.h"
#include "TsDx11Core.Internal.h"

namespace TS
{
    enum class FormatType : unsigned
    {
        UNKNOWN = 0,
        UINT32 = 1,
        SINT32 = 2,
        FLOAT32 = 3,
        UNORM = 4,
    };

    DXGI_FORMAT ConvertFormat(unsigned dataCount, FormatType type)
    {
        if (type == FormatType::FLOAT32)
        {
            if (dataCount == 1) return DXGI_FORMAT_R32_FLOAT;
            if (dataCount == 2) return DXGI_FORMAT_R32G32_FLOAT;
            if (dataCount == 3) return DXGI_FORMAT_R32G32B32_FLOAT;
            if (dataCount == 4) return DXGI_FORMAT_R32G32B32A32_FLOAT;
        }
        if (type == FormatType::UINT32)
        {
            if (dataCount == 1) return DXGI_FORMAT_R32_UINT;
            if (dataCount == 2) return DXGI_FORMAT_R32G32_UINT;
            if (dataCount == 3) return DXGI_FORMAT_R32G32B32_UINT;
            if (dataCount == 4) return DXGI_FORMAT_R32G32B32A32_UINT;
        }
        if (type == FormatType::SINT32)
        {
            if (dataCount == 1) return DXGI_FORMAT_R32_SINT;
            if (dataCount == 2) return DXGI_FORMAT_R32G32_SINT;
            if (dataCount == 3) return DXGI_FORMAT_R32G32B32_SINT;
            if (dataCount == 4) return DXGI_FORMAT_R32G32B32A32_SINT;
        }

        if (type == FormatType::UNORM)
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
            , ConvertFormat(dataCount,FormatType::FLOAT32)
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
            , ConvertFormat(dataCount,FormatType::UINT32)
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
            , ConvertFormat(dataCount,FormatType::UNORM)
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
            , ConvertFormat(dataCount,FormatType::SINT32)
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

    SmartArray<InputElementDesc> MakeInputLayoutDescFromMemory(Binary& binary)
    {
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
        SmartArray<char*> names(cntvariable);
		SmartArray<unsigned> index(cntvariable);
		SmartArray<DXGI_FORMAT> format(cntvariable);
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

            format[i] = ConvertFormat(dataCount, (FormatType)(type));
        }
        cntvariable -= systemSemantices;

        SmartArray<TS::InputElementDesc> descs(new TS::InputElementDesc[cntvariable], cntvariable);
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

        return descs;
    }
}