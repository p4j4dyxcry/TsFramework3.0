#pragma once

namespace TS
{
    //-------------------------------------------------------------------------------------
    struct ShaderResourceView
    {
        ID3D11ShaderResourceView * Srv;
        unsigned RegisterID;

        ShaderResourceView(ID3D11ShaderResourceView * _shaderResourceView, unsigned _register)
        {
            Srv = _shaderResourceView;
            RegisterID = _register;
        }
    };

    //-------------------------------------------------------------------------------------
    class RenderTarget : public ID3DResourcesHolder
    {
    public:
        RenderTarget();
        RenderTarget& Initialize(TextureDesc& desc, TsSharedPtr<ID3D11Device>&device);
        RenderTarget& Initialize(TsSharedPtr<IDXGISwapChain>& pSwapChain, TsSharedPtr<ID3D11Device>& device);
        TsSharedPtr<ID3D11RenderTargetView> GetD3DRenderTargetView() const;
        TsSharedPtr<ID3D11ShaderResourceView> GetD3DShaderResourceView() const;
        DXGI_FORMAT GetFormat()const;
        TsSharedPtr<ID3D11Resource> GetD3D11Resource() const override;
        ShaderResourceView ToShaderResourceView(unsigned registerIndex)const;
        TextureDesc GetDesc() const;
        HashCode GetHash()const;
    private:
        TsSharedPtr<ID3D11RenderTargetView> _rtv;
        TsSharedPtr<ID3D11ShaderResourceView> _srv;
        TsSharedPtr<ID3D11Texture2D> _texture;
        TextureDesc _desc;
    };

    //-------------------------------------------------------------------------------------
    class DepthStencilTarget : public ID3DResourcesHolder
    {
    public:
        DepthStencilTarget();
        DepthStencilTarget& Initialize(TextureDesc& desc, TsSharedPtr<ID3D11Device>& device);
        TsSharedPtr<ID3D11DepthStencilView>GetD3DDepthStencilView() const;
        TsSharedPtr<ID3D11ShaderResourceView> GetD3DShaderResourceView() const;
        DXGI_FORMAT GetFormat()const;
        TsSharedPtr<ID3D11Resource> GetD3D11Resource() const override;
        ShaderResourceView ToShaderResourceView(unsigned registerIndex)const;
        HashCode GetHash()const;
    private:
        TsSharedPtr<ID3D11DepthStencilView> _dsv;
        TsSharedPtr<ID3D11ShaderResourceView> _srv;
        TsSharedPtr<ID3D11Texture2D> _texture;
        TextureDesc _desc;
    };

    //-------------------------------------------------------------------------------------
    class DepthStencilState
    {
    public:
        DepthStencilState();
        DepthStencilState& Initialize(DepthStencilStateDesc& desc, TsSharedPtr<ID3D11Device>& device);
        TsSharedPtr<ID3D11DepthStencilState> GetDepthStencilState() const;
        HashCode GetHash()const;
    private:
        TsSharedPtr<ID3D11DepthStencilState> _depthStencilState;
        DepthStencilStateDesc _desc;
    };

    //-------------------------------------------------------------------------------------
    class RasterizerState
    {
    public:
        RasterizerState();
        RasterizerState& Initialize(RasterizerStateDesc& desc, TsSharedPtr<ID3D11Device>& device);
        TsSharedPtr<ID3D11RasterizerState> GetD3DRasterizerState() const;
        D3D11_VIEWPORT& ViewPort();
        HashCode GetHash()const;
    private:
        D3D11_VIEWPORT _viewport;
        TsSharedPtr<ID3D11RasterizerState> _rasterizerState;
        RasterizerStateDesc _desc;
    };

    //-------------------------------------------------------------------------------------
    class BlendState
    {
    public:
        BlendState();
        BlendState& Initialize(BlendStateDesc& desc, TsSharedPtr<ID3D11Device>& device);
        TsSharedPtr<ID3D11BlendState> GetD3DBlendState() const;
        HashCode GetHash()const;
    private:
        TsSharedPtr<ID3D11BlendState> _blendState;
        BlendStateDesc _desc;
    };

    //-------------------------------------------------------------------------------------
    class SamplerState
    {
    public:
        SamplerState();
        SamplerState& Initialize(SamplerStateDesc& desc, TsSharedPtr<ID3D11Device>& device);
        TsSharedPtr<ID3D11SamplerState> GetD3DBlendState() const;
        HashCode GetHash()const;
    private:
        TsSharedPtr<ID3D11SamplerState> _samplerState;
        SamplerStateDesc _desc;
    };
}