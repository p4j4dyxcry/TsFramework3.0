#include "pch.h"
#include "TsDx11Core.Internal.h"

namespace TS
{
    //-------------------------------------------------------------------------------------------
    //! RenderTarget
    RenderTarget::RenderTarget() : _rtv(nullptr), _srv(nullptr), _texture(nullptr)
    {

    }

    RenderTarget& RenderTarget::Initialize(TextureDesc& desc, TS::SharedPtr<ID3D11Device>& device)
    {
        _desc = desc;
        auto pDevice = device;
        {
            D3D11_TEXTURE2D_DESC texDesc;
            texDesc.ArraySize = 1;
            texDesc.CPUAccessFlags = _desc.CpuAccessFlag;
            texDesc.MipLevels = 1;
            texDesc.MiscFlags = 0;
            texDesc.SampleDesc.Count = 1;
            texDesc.SampleDesc.Quality = 0;
            texDesc.Usage = D3D11_USAGE_DEFAULT;

            texDesc.Width = _desc.Width;
            texDesc.Height = _desc.Height;
            texDesc.Format = _desc.Format;

            texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

            ID3D11Texture2D * pTex2D;
            HRESULT hr = pDevice->CreateTexture2D(&texDesc, nullptr, &pTex2D);
            Error::Make(hr).DoError(true);

            _texture = _makeD3DShared(pTex2D);
        }

        {
            ID3D11RenderTargetView * pRTV;
            HRESULT hr = pDevice->CreateRenderTargetView(_texture.get(), nullptr, &pRTV);
            Error::Make(hr).DoError(true);
            _rtv = _makeD3DShared(pRTV);
        }

        {
            ID3D11ShaderResourceView * pSRV;
            HRESULT hr = pDevice->CreateShaderResourceView(_texture.get(), nullptr, &pSRV);
            Error::Make(hr).DoError(true);
            _srv = _makeD3DShared(pSRV);
        }
        return *this;
    }

    RenderTarget& RenderTarget::Initialize(TS::SharedPtr<IDXGISwapChain>& pSwapChain, TS::SharedPtr<ID3D11Device>& device)
    {
        ID3D11Texture2D * pTexture;
        HRESULT hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pTexture));
        Error::Make(hr).DoError(true);

        ID3D11RenderTargetView * pRtv;
        hr = device->CreateRenderTargetView(pTexture, nullptr, &pRtv);

        _texture = _makeD3DShared(pTexture);
        _rtv = _makeD3DShared(pRtv);

        Error::Make(hr).DoError(true);

        D3D11_TEXTURE2D_DESC desc;
        _texture->GetDesc(&desc);

        _desc.Format = desc.Format;
        _desc.Width = desc.Width;
        _desc.Height = desc.Height;
        _desc.CpuAccessFlag = desc.CPUAccessFlags;
        _desc.Id = 2001;

        {
            ID3D11ShaderResourceView * pSRV;
            HRESULT hr = device->CreateShaderResourceView(_texture.get(), nullptr, &pSRV);
            Error::Make(hr).DoError(true);
            _srv = _makeD3DShared(pSRV);
        }

        return *this;
    }

    TS::SharedPtr<ID3D11RenderTargetView> RenderTarget::GetD3DRenderTargetView() const
    {
        return _rtv;
    }

    TS::SharedPtr<ID3D11ShaderResourceView> RenderTarget::GetD3DShaderResourceView() const
    {
        return _srv;
    }

    DXGI_FORMAT RenderTarget::GetFormat() const
    {
        return _desc.Format;
    }

    TS::SharedPtr<ID3D11Resource> RenderTarget::GetD3D11Resource() const
    {
        return _texture;
    }

    ShaderResourceView RenderTarget::ToShaderResourceView(unsigned registerIndex)const
    {
        return { _srv.get(), registerIndex };
    }

    TextureDesc RenderTarget::GetDesc() const
    {
        return _desc;
    }

    HashCode RenderTarget::GetHash() const
    {
        return _getHashCode(_desc);
    }

    //-------------------------------------------------------------------------------------------
    //! DepthStencilTarget
    DepthStencilTarget::DepthStencilTarget() : _dsv(nullptr), _srv(nullptr), _texture(nullptr)
    {
    }

    DepthStencilTarget& DepthStencilTarget::Initialize(TextureDesc& desc, TS::SharedPtr<ID3D11Device>& device)
    {
        _desc = desc;

        ID3D11Texture2D * pTex2D;
        {
            D3D11_TEXTURE2D_DESC texDesc;
            texDesc.ArraySize = 1;
            texDesc.CPUAccessFlags = _desc.CpuAccessFlag;
            texDesc.MipLevels = 1;
            texDesc.MiscFlags = 0;
            texDesc.SampleDesc.Count = 1;
            texDesc.SampleDesc.Quality = 0;
            texDesc.Usage = D3D11_USAGE_DEFAULT;

            texDesc.Width = _desc.Width;
            texDesc.Height = _desc.Height;
            texDesc.Format = _desc.Format;

            texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

            HRESULT hr = device->CreateTexture2D(&texDesc, nullptr, &pTex2D);
            Error::Make(hr).DoError(true);

            _texture = _makeD3DShared(pTex2D);
        }

        //Depth Stencil View 
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC desc;
            memset(&desc, 0, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
            desc.Format = _desc.Format;
            desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            desc.Texture2D.MipSlice = 0;

            ID3D11DepthStencilView * pDsv;
            HRESULT hr = device->CreateDepthStencilView(pTex2D, &desc, &pDsv);
            Error::Make(hr).DoError(true);
            _dsv = _makeD3DShared(pDsv);
        }
        return *this;
    }

    TS::SharedPtr<ID3D11DepthStencilView> DepthStencilTarget::GetD3DDepthStencilView() const
    {
        return _dsv;
    }

    TS::SharedPtr<ID3D11ShaderResourceView> DepthStencilTarget::GetD3DShaderResourceView() const
    {
        return _srv;
    }

    DXGI_FORMAT DepthStencilTarget::GetFormat() const
    {
        return _desc.Format;
    }

    TS::SharedPtr<ID3D11Resource> DepthStencilTarget::GetD3D11Resource() const
    {
        return _texture;
    }

    ShaderResourceView DepthStencilTarget::ToShaderResourceView(unsigned registerIndex)const
    {
        return { _srv.get(), registerIndex };
    }

    HashCode DepthStencilTarget::GetHash() const
    {
        return _getHashCode(_desc);
    }

    //-------------------------------------------------------------------------------------------
    //! DepthStencilState

    DepthStencilState::DepthStencilState() :_depthStencilState(nullptr)
    {
    }

    DepthStencilState& DepthStencilState::Initialize(DepthStencilStateDesc& desc, TS::SharedPtr<ID3D11Device>& device)
    {
        _desc = desc;
        D3D11_DEPTH_STENCIL_DESC d3dDesc;
        memset(&d3dDesc, 0, sizeof(D3D11_DEPTH_STENCIL_DESC));
        d3dDesc.DepthWriteMask = static_cast<D3D11_DEPTH_WRITE_MASK>(desc.ZWriteEnable);
        d3dDesc.DepthEnable = desc.ZEnable;
        d3dDesc.DepthFunc = static_cast<D3D11_COMPARISON_FUNC>(desc.Compare);
        ID3D11DepthStencilState * pDss;
        HRESULT hr = device->CreateDepthStencilState(&d3dDesc, &pDss);
        Error::Make(hr).DoError(true);
        _depthStencilState = _makeD3DShared(pDss);
        return *this;
    }

    TS::SharedPtr<ID3D11DepthStencilState> DepthStencilState::GetDepthStencilState() const
    {
        return _depthStencilState;
    }

    HashCode DepthStencilState::GetHash() const
    {
        return _getHashCode(_desc);
    }

    //-------------------------------------------------------------------------------------------
    //! RasterizerState

    RasterizerState::RasterizerState() :_rasterizerState(nullptr)
    {

    }

    RasterizerState& RasterizerState::Initialize(RasterizerStateDesc& desc, TS::SharedPtr<ID3D11Device>& device)
    {
        _desc = desc;
        D3D11_RASTERIZER_DESC rasterDesc;

        rasterDesc.AntialiasedLineEnable = desc.UseAntiAlias;
        rasterDesc.CullMode = static_cast<D3D11_CULL_MODE>(desc.Culling);
        rasterDesc.FillMode = static_cast<D3D11_FILL_MODE>(desc.Fill);
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.SlopeScaledDepthBias = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.ScissorEnable = false;

        _viewport = desc.Viewport;
        ID3D11RasterizerState * pRss;
        HRESULT hr = device->CreateRasterizerState(&rasterDesc, &pRss);       
        Error::Make(hr).DoError(true);
        _rasterizerState = _makeD3DShared(pRss);
        return *this;
    }

    TS::SharedPtr<ID3D11RasterizerState> RasterizerState::GetD3DRasterizerState() const
    {
        return _rasterizerState;
    }

    D3D11_VIEWPORT& RasterizerState::ViewPort()
    {
        return _viewport;
    }

    HashCode RasterizerState::GetHash() const
    {
        return _getHashCode(_desc);
    }

    //-------------------------------------------------------------------------------------------
    //! BlendState

    BlendState::BlendState() :_blendState(nullptr)
    {
    }

    BlendState& BlendState::Initialize(BlendStateDesc& blendDesc, TS::SharedPtr<ID3D11Device>& device)
    {
        _desc = blendDesc;
        D3D11_BLEND_DESC desc;
        // zero clear
        memset(&desc, 0, sizeof(desc));
        for (int i = 1; i < 8; ++i)
        {
            desc.RenderTarget[i].BlendEnable = false;
            desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
            desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
            desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
            desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
            desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        }

        desc.AlphaToCoverageEnable = false;

        desc.IndependentBlendEnable = false;

        if (blendDesc.Mode == BlendMode::NONE)
            desc.RenderTarget[0].BlendEnable = false;
        else
        {
            desc.RenderTarget[0].BlendEnable = true;
            desc.IndependentBlendEnable = true;
        }

        switch (blendDesc.Mode)
        {
        case BlendMode::ALPHA_BLEND:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            break;
        case BlendMode::ADD:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            break;
        case BlendMode::SUBTRACT:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_SUBTRACT;
            break;
        default:
            desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
            desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
            desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            break;
        }

        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        ID3D11BlendState * pBss;
        HRESULT hr = device->CreateBlendState(&desc, &pBss);
        Error::Make(hr).DoError(true);
        _blendState = _makeD3DShared(pBss);
        return *this;
    }

    TS::SharedPtr<ID3D11BlendState> BlendState::GetD3DBlendState() const
    {
        return _blendState;
    }

    HashCode BlendState::GetHash() const
    {
        return _getHashCode(_desc);
    }

    //-------------------------------------------------------------------------------------------
    //! SamplerState

    SamplerState::SamplerState() :_samplerState(nullptr)
    {
    }

    SamplerState& SamplerState::Initialize(SamplerStateDesc& desc, TS::SharedPtr<ID3D11Device>& device)
    {
        _desc = desc;
        ID3D11SamplerState * pSS;
        HRESULT hr = device->CreateSamplerState(reinterpret_cast<D3D11_SAMPLER_DESC*>(&desc), &pSS);
        _samplerState = _makeD3DShared(pSS);
        Error::Make(hr).DoError(true);
        return *this;
    }

    TS::SharedPtr<ID3D11SamplerState> SamplerState::GetD3DBlendState() const
    {
        return _samplerState;
    }

    HashCode SamplerState::GetHash() const
    {
        return _getHashCode(_desc);
    }
}
