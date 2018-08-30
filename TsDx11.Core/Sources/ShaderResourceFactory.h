#pragma once

namespace TS
{
    //-------------------------------------------------------------------------------------
    class ShaderResourceFactory
    {
    public:
        ShaderResourceFactory();
        ShaderResourceFactory Initialize(DeviceHolder& _deviceHolder);

        RenderTarget& CreateRenderTarget(TextureDesc& desc);
        RenderTarget& CreateBackBufferRenderTarget();
        DepthStencilTarget& CreateDepthStencilTarget(TextureDesc& desc);
        DepthStencilState& CreateDepthStencilState(DepthStencilStateDesc& desc);
        RasterizerState& CreateRasterizerState(RasterizerStateDesc& desc);
        BlendState& CreateBlendState(BlendStateDesc& desc);
        SamplerState& CreateSamplerState(SamplerStateDesc& desc);

        RenderTarget& GetRenderTarget(HashCode hash);
        DepthStencilTarget& GetDepthStencilTarget(HashCode hash);
        DepthStencilState& GetDepthStencilState(HashCode hash);
        RasterizerState& GetRasterizerState(HashCode hash);
        BlendState& GetBlendState(HashCode hash);
        SamplerState& GetSamplerState(HashCode hash);

    private:
        DeviceHolder                           _deviceHolder;
        LruCache<HashCode, RenderTarget>       _renderTargetCache;
        LruCache<HashCode, DepthStencilTarget> _depthStencilTarget;
        LruCache<HashCode, DepthStencilState>  _depthStencilStateCache;
        LruCache<HashCode, RasterizerState>    _rasterizerStateCache;
        LruCache<HashCode, BlendState>         _blendStateCache;
        LruCache<HashCode, SamplerState>       _samplerStateCache;
        HashCode _backBufferRenderTargetHash;
    };
}
