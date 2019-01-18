#pragma once

namespace TS
{
    enum class ShaderType : unsigned
    {
        Unknown,
        Vertex = 0x01,
        Pixel = 0x02,
        Geometry = 0x04,
        Hull = 0x08,
        Domain = 0x0f,
        Compute = 0x10,
    };

    inline ShaderType operator&(const TS::ShaderType& lhs, const TS::ShaderType& rhs)
    {
        return static_cast<ShaderType>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs));
    }
    inline ShaderType operator|(const TS::ShaderType& lhs, const TS::ShaderType& rhs)
    {
        return static_cast<ShaderType>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
    }


    template<typename T>
    class D3DSharedWrapper
    {
    public:
        D3DSharedWrapper(TS::SharedPtr<T> shader = nullptr) { _ptr = shader; }
        TS::SharedPtr<T> Get() { return _ptr; }
    private:
        TS::SharedPtr<T> _ptr;
    };

    using VertexShader = D3DSharedWrapper<ID3D11VertexShader>;
    using PixelShader = D3DSharedWrapper<ID3D11PixelShader>;
    using GeometryShader = D3DSharedWrapper<ID3D11GeometryShader>;
    using HullShader = D3DSharedWrapper<ID3D11HullShader>;
    using DomainShader = D3DSharedWrapper<ID3D11DomainShader>;
    using ComputeShader = D3DSharedWrapper<ID3D11ComputeShader>;
    using InputLayout = D3DSharedWrapper<ID3D11InputLayout>;

    enum DeviceContextMapMode
    {
        Ts_Map_Read = 1,
        Ts_Map_Write = 2,
        Ts_Map_Rw = 3,
        Ts_Map_WriteDiscard = 4,
        Ts_Map_WriteNoOverride = 5
    };

    struct MappedSubResource
    {
        void *pData;
        unsigned RowPitch;
        unsigned DepthPitch;

        MappedSubResource(void* data, unsigned row, unsigned depth)
            :pData(data), RowPitch(row), DepthPitch(depth) {}
    };

    struct ID3DResourcesHolder
    {
        virtual ~ID3DResourcesHolder() = default;
        virtual TS::SharedPtr<ID3D11Resource> GetD3D11Resource()const = 0;
    };

    enum class ComparisonFunc : unsigned
    {
        NEVER = 1,        //&0 
        LESS = 2,         //<= 
        EQUAL = 3,        //== 
        LESS_EQUAL = 4,   //<= 
        GREATER = 5,      //>  
        NOT_EQUAL = 6,    //!= 
        GREATER_EQUAL = 7,//>= 
        ALWAYS = 8,       //|1 
    };

    enum class CullMode : unsigned
    {
        BACK = 3,
        FRONT = 2,
        NONE = 1
    };

    enum class FillMode : unsigned
    {
        SOLID = 3,
        WIREFRAME = 2
    };

    enum class FilterMode : unsigned
    {
        POINT = 0,
        LINEAR = 1,
    };

    enum class TextureAddresMode : unsigned
    {
        WRAP = 1,
        MIRROR = 2,
        CLAMP = 3,
        BORDER = 4,
        MIRROR_ONCE = 5
    };

    enum class BlendMode : unsigned
    {
        NONE = 0,
        ALPHA_BLEND = 1,
        ADD = 2,
        SUBTRACT = 3,
    };

    struct DepthStencilStateDesc
    {
        bool ZWriteEnable = true;
        bool ZEnable = true;
        unsigned Id = 0;
        ComparisonFunc Compare = ComparisonFunc::LESS;
    };

    struct RasterizerStateDesc
    {
        CullMode Culling = CullMode::BACK;
        FillMode Fill = FillMode::SOLID;
        D3D11_VIEWPORT Viewport;
        bool UseAntiAlias = true;

        RasterizerStateDesc(unsigned w = 0, unsigned h = 0)
        {
            Viewport.Height = static_cast<float>(h);
            Viewport.Width = static_cast<float>(w);
            Viewport.MaxDepth = 1;
            Viewport.MinDepth = 0;
            Viewport.TopLeftX = 0;
            Viewport.TopLeftY = 0;
        }
    };

    struct TextureDesc
    {
        unsigned Width;
        unsigned Height;
        unsigned CpuAccessFlag = 0;
        unsigned Id = 0;
        DXGI_FORMAT Format;

        TextureDesc(unsigned w = -0, unsigned h = -0, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM)
        {
            Width = w;
            Height = h;
            Format = format;
        }
    };

    struct SamplerStateDesc
    {
        FilterMode Filter;
        TextureAddresMode AddressU;
        TextureAddresMode AddressV;
        TextureAddresMode AddressW;
        float MipLODBias;
        unsigned MaxAnisotropy;
        ComparisonFunc Compare;
        float BorderColor[4]{ 0,0,0,0 };
        float MinLOD = 0;
        float MaxLOD;
    };

    using InputElementDesc = D3D11_INPUT_ELEMENT_DESC;

    struct BlendStateDesc
    {
        BlendMode Mode = BlendMode::NONE;
    };
}