#pragma once
//--------------------------------------------
//! 共通定義
//--------------------------------------------

namespace TS
{
    //-------------------------------------------------------
    //! 文字コード
    #ifdef UNICODE
        using TsChar = wchar_t;
        #ifndef _T
            #define _T(__char__string__) L#__char__string__
        #endif
    #else
        using TsChar = char;
        #ifndef _T
            #define _T(__char__string__) __char__string__
        #endif
    #endif

    //-------------------------------------------------------
    //! メモリ解放
    #ifndef SAFE_RELEASE
        #define SAFE_RELEASE( ptr )\
        {\
	        if( (ptr) != nullptr )\
	        {\
		        (ptr)->Release();\
		        (ptr) = nullptr;\
	        }\
        }
    #endif

    #ifndef SAFE_DELETE
        #define SAFE_DELETE( ptr )\
        {\
	        if( ptr != nullptr )\
	        {\
		        delete ptr;\
		        ptr = nullptr;\
	        }\
        }
    #endif   
    
    //! スマートポインタ用のカスタムデリータ―
    template<typename T>
    class ReleaseDeleter
    {
    public:
        void operator()(T*& _ptr)
        {
            if (_ptr)
                _ptr->Release();
            _ptr = nullptr;
        }
    };
          
    //-------------------------------------------------------
    //! 開発支援

    #define TS_CLASS_DISABLE_COPY(name) name(const name&) = delete;name& operator =(const name&) = delete
    #define TS_CLASS_DISABLE_MOVE(name) name(const name&&) = delete;name& operator =(const name&&) = delete

    enum ErrorLevel : unsigned
    {
        Ts_Unknown,
        Ts_Success,
        Ts_Develop,
        Ts_Warning,
        Ts_Error,
    };

    class ILogger
    {
    public:
        virtual ~ILogger() = default;
        virtual void Log(const TsChar* message, ErrorLevel errorLevel) = 0;
    };

    struct ErrorResult
    {
        friend class Error;
    public:

        bool IsError() const;

        const TsChar* GetErrorMessage() const;

        ErrorLevel GetErrorLevel() const;

        ErrorResult Log() const;

        bool DoError(bool isThrow = true) const;

        ErrorResult Do()const;

    private :
        ErrorResult(const TS::TsChar* message = _T(""), const ErrorLevel error_level = ErrorLevel::Ts_Error, ILogger* logger = nullptr);

    private:
        const TS::TsChar * _message;
        TS::ErrorLevel _errorLevel;
        ILogger* _logger;
    };

    class Error
    {
    public:
        static ErrorResult Success;

        static ErrorResult Make(HRESULT hResult);

        static ErrorResult Make(const TS::TsChar* message = _T(""), const ErrorLevel error_level = ErrorLevel::Ts_Error);

        static ErrorResult Assert(bool _true, const TS::TsChar* message = _T("Assert に失敗しました"));
        static void SetLogger(ILogger* logger);

    private:
        static ILogger* _loger;
    };


    //-------------------------------------------------------
    //! 基本型

    #define TsSharedPtr  std::shared_ptr
    #define TsWeakptr    std::weak_ptr   
    #define TsMakeShared std::make_shared
    #define TsList       std::list
    #define TsMap        std::unordered_map

    #include "LruCache.h"

    using HashCode = unsigned long long;

    template<typename T>
    struct Array
    {
        T* _data;
        size_t _size;
        Array(T* data = nullptr, size_t sz = 0) :_data(data), _size(sz) {}

        inline T& operator[](unsigned i) { return _data[i]; }

        void Delete() { SAFE_DELETE(_data); _size = 0; }
    };

    struct Binary : Array<unsigned char>
    {
        Binary(unsigned char* data, size_t sz) : Array<unsigned char>(data, sz){}
    };
    
    //-------------------------------------------------------
    //! グラフィックス
    enum ShaderType
    {
        Ts_Vertex = 0x01,
        Ts_Pixel = 0x02,
        Ts_Geometry = 0x04,
        Ts_Hull = 0x08,
        Ts_Domain = 0x0f,
        Ts_Compute = 0x10,
        ShaderType_Unknown,
    };

    template<typename T>
    class D3DSharedWrapper
    {
    public:
        D3DSharedWrapper(TsSharedPtr<T> shader = nullptr){ _ptr = shader;}
        TsSharedPtr<T> Get(){return _ptr;}
    private:
        TsSharedPtr<T> _ptr;
    };

    using VertexShader   = D3DSharedWrapper<ID3D11VertexShader>;
    using PixelShader    = D3DSharedWrapper<ID3D11PixelShader>;
    using GeometryShader = D3DSharedWrapper<ID3D11GeometryShader>;
    using HullShader     = D3DSharedWrapper<ID3D11HullShader>;
    using DomainShader   = D3DSharedWrapper<ID3D11DomainShader>;
    using ComputeShader  = D3DSharedWrapper<ID3D11ComputeShader>;
    using InputLayout    = D3DSharedWrapper<ID3D11InputLayout>;
    
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
        virtual TsSharedPtr<ID3D11Resource> GetD3D11Resource()const = 0;
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
        bool ZEnable = true ;
        unsigned Id = 0;
        ComparisonFunc Compare = ComparisonFunc::LESS;
    };
    
    struct RasterizerStateDesc
    {
        CullMode Culling = CullMode::BACK;
        FillMode Fill = FillMode::SOLID;
        D3D11_VIEWPORT Viewport;
        bool UseAntiAlias = true;

        RasterizerStateDesc(unsigned w =0, unsigned h =0)
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

        TextureDesc(unsigned w=-0, unsigned h=-0 , DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM)
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
        float BorderColor[4]{0,0,0,0};
        float MinLOD = 0;
        float MaxLOD;
    };

    using InputElementDesc = D3D11_INPUT_ELEMENT_DESC;

    struct BlendStateDesc 
    {
        BlendMode Mode = BlendMode::NONE;
    };

}