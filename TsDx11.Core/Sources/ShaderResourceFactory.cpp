#include "pch.h"
#include "TsDx11Core.Internal.h"

using namespace TS;

//-------------------------------------------------------------------------------------------
//! ResourceFactory
ShaderResourceFactory::ShaderResourceFactory()
    :_renderTargetCache(LruCache<HashCode, RenderTarget>(256)),
    _depthStencilTarget(LruCache<HashCode, DepthStencilTarget>(256)),
    _depthStencilStateCache(LruCache<HashCode, DepthStencilState>(32)),
    _rasterizerStateCache(LruCache<HashCode, RasterizerState>(32)),
    _blendStateCache(LruCache<HashCode, BlendState>(8)),
    _samplerStateCache(LruCache<HashCode, SamplerState>(32)),
    _backBufferRenderTargetHash(0)
{
}

TS::ShaderResourceFactory TS::ShaderResourceFactory::Initialize(DeviceHolder& deviceHolder)
{
    _deviceHolder = deviceHolder;
    return *this;
}

RenderTarget& ShaderResourceFactory::CreateRenderTarget(TextureDesc& desc)
{
    const auto hash = _getHashCode(desc);
    if (_renderTargetCache.Contains(hash))
        return _renderTargetCache[hash];

    _renderTargetCache.Register(hash, RenderTarget().Initialize(desc, _deviceHolder.GetD3D11Device()));
    return _renderTargetCache[hash];
}

RenderTarget& ShaderResourceFactory::CreateBackBufferRenderTarget()
{
    if (_backBufferRenderTargetHash == 0)
    {
        auto _renderTarget = RenderTarget();
        _renderTarget.Initialize(_deviceHolder.GetD3DSwapChain(), _deviceHolder.GetD3D11Device());
        auto desc = _renderTarget.GetDesc();
        _backBufferRenderTargetHash = _getHashCode(desc);
        _renderTargetCache.Register(_backBufferRenderTargetHash, _renderTarget);
    }
    return _renderTargetCache[_backBufferRenderTargetHash];
}

DepthStencilTarget& ShaderResourceFactory::CreateDepthStencilTarget(TextureDesc& desc)
{
    const auto hash = _getHashCode(desc);
    if (_depthStencilTarget.Contains(hash))
        return _depthStencilTarget[hash];

    auto data = DepthStencilTarget();
    data.Initialize(desc, _deviceHolder.GetD3D11Device());
    _depthStencilTarget.Register(hash, data);
    return _depthStencilTarget[hash];
}

DepthStencilState& ShaderResourceFactory::CreateDepthStencilState(DepthStencilStateDesc& desc)
{
    const auto hash = _getHashCode(desc);
    if (_renderTargetCache.Contains(hash))
        return _depthStencilStateCache[hash];

    auto data = DepthStencilState();
    data.Initialize(desc, _deviceHolder.GetD3D11Device());
    _depthStencilStateCache.Register(hash, data);
    return _depthStencilStateCache[hash];
}

RasterizerState& ShaderResourceFactory::CreateRasterizerState(RasterizerStateDesc& desc)
{
    const auto hash = _getHashCode(desc);
    if (_rasterizerStateCache.Contains(hash))
        return _rasterizerStateCache[hash];

    auto data = RasterizerState();
    data.Initialize(desc, _deviceHolder.GetD3D11Device());
    _rasterizerStateCache.Register(hash, data);
    return _rasterizerStateCache[hash];
}

BlendState& ShaderResourceFactory::CreateBlendState(BlendStateDesc& desc)
{
    const auto hash = _getHashCode(desc);
    if (_blendStateCache.Contains(hash))
        return _blendStateCache[hash];

    auto data = BlendState();
    data.Initialize(desc, _deviceHolder.GetD3D11Device());
    _blendStateCache.Register(hash, data);
    return _blendStateCache[hash];
}

SamplerState& ShaderResourceFactory::CreateSamplerState(SamplerStateDesc& desc)
{
    const auto hash = _getHashCode(desc);
    if (_samplerStateCache.Contains(hash))
        return _samplerStateCache[hash];

    auto data = SamplerState();
    data.Initialize(desc, _deviceHolder.GetD3D11Device());
    _samplerStateCache.Register(hash,data);
    return _samplerStateCache[hash];
}

RenderTarget& ShaderResourceFactory::GetRenderTarget(HashCode hash)
{
    return _renderTargetCache[hash];
}

DepthStencilTarget& ShaderResourceFactory::GetDepthStencilTarget(HashCode hash)
{
    return _depthStencilTarget[hash];
}

DepthStencilState& ShaderResourceFactory::GetDepthStencilState(HashCode hash)
{
    return _depthStencilStateCache[hash];
}

RasterizerState& ShaderResourceFactory::GetRasterizerState(HashCode hash)
{
    return _rasterizerStateCache[hash];
}

BlendState& ShaderResourceFactory::GetBlendState(HashCode hash)
{
    return _blendStateCache[hash];
}

SamplerState& ShaderResourceFactory::GetSamplerState(HashCode hash)
{
    return _samplerStateCache[hash];
}
