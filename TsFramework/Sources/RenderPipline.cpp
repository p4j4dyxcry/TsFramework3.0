#include "pch.h"
#include "RenderPipline.h"

using namespace TS;

TS::GfxPipline::GfxPipline(DeviceHolder& holder, ShaderResourceFactory& factory)
    :_deviceHolder(holder),_shaderResourceFactory(factory)
{
}
TS::GfxPipline& TS::GfxPipline::LoadVertexShader(const TsChar* filename)
{
	auto binary = _loadCompiledShader(filename);
	auto&& elementDescs = MakeInputLayoutDescFromMemory(binary);
	_vertexShader = _deviceHolder.CreateVertexShader(binary);
	_inputLayout = _deviceHolder.CreateInputLayout(binary, elementDescs, elementDescs.Length());
    return *this;
}

TS::GfxPipline& TS::GfxPipline::LoadPixelShader(const TsChar* filename)
{
    const auto binary = _loadCompiledShader(filename);
	_pixelShader = _deviceHolder.CreatePixelShader(binary);
    return *this;
}

GfxPipline & TS::GfxPipline::SetRasterizerState(RasterizerStateDesc & desc)
{
	_shaderResourceFactory.CreateRasterizerState(desc);
	_rasterizerState = _getHashCode(desc);
	return *this;
}

GfxPipline & TS::GfxPipline::SetDepthStencilState(DepthStencilStateDesc & desc)
{
	_shaderResourceFactory.CreateDepthStencilState(desc);
	_depthStencilState = _getHashCode(desc);
	return *this;
}

GfxPipline & TS::GfxPipline::SetBlendState(BlendStateDesc & desc)
{
	_shaderResourceFactory.CreateBlendState(desc);
	_blendState = _getHashCode(desc);
	return *this;
}

GfxPipline & TS::GfxPipline::SetRenderTarget(TextureDesc & desc, int slot)
{
	_shaderResourceFactory.CreateRenderTarget(desc);
	_renderTargets[slot] = _getHashCode(desc);
	return *this;
}

GfxPipline & TS::GfxPipline::SetDepthStencilTarget(TextureDesc & desc)
{
	_shaderResourceFactory.CreateDepthStencilTarget(desc);
	_depthStencilView = _getHashCode(desc);
	return *this;
}

GfxPipline & TS::GfxPipline::SetClearColor(float r, float g, float b, float a)
{
	_clearColor = Vector4(r,g,b,a);
	return *this;
}

GfxPipline & TS::GfxPipline::Apply()
{
	auto& deviceContext = _deviceHolder.ImmediateContext();
	static const Handle invalid_Hash = 0;

	unsigned _index = 0;
	for (auto rt : _renderTargets)
		if(rt != invalid_Hash)
			deviceContext.SetRenderTarget(_shaderResourceFactory.GetRenderTarget(rt), _index++);
	if (_depthStencilView != invalid_Hash)
		deviceContext.SetDepthStencilTarget(_shaderResourceFactory.GetDepthStencilTarget(_depthStencilView));

    deviceContext.ApplyBufferTargets();

    deviceContext.ClearColorToRenderTaegets(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
	deviceContext.ClearDepth();

	if (_blendState != invalid_Hash)
		deviceContext.SetBlendState(_shaderResourceFactory.GetBlendState(_blendState));
	if (_rasterizerState != invalid_Hash)
		deviceContext.SetRasterizerState(_shaderResourceFactory.GetRasterizerState(_rasterizerState));
	if (_depthStencilState != invalid_Hash)
		deviceContext.SetDepthStencilState(_shaderResourceFactory.GetDepthStencilState(_depthStencilState));
	
	deviceContext.SetInputLayout(_inputLayout)
                 .SetVertxShader(_vertexShader)
				 .SetPixelShader(_pixelShader);

	return *this;
}

GfxPipline& GfxPipline::SetupDefault()
{
    auto renderTargetDesc = _shaderResourceFactory.CreateBackBufferRenderTarget().GetDesc();

    auto depthStencilTextureDesc = TextureDesc(renderTargetDesc.Width, renderTargetDesc.Height, DXGI_FORMAT_D24_UNORM_S8_UINT);
    auto depthStencilStateDesc = DepthStencilStateDesc();
    auto rasterizerStateDesc = RasterizerStateDesc(renderTargetDesc.Width, renderTargetDesc.Height);

    SetRenderTarget(renderTargetDesc, 0);
    SetDepthStencilTarget(depthStencilTextureDesc);
    SetDepthStencilState(depthStencilStateDesc);
    SetRasterizerState(rasterizerStateDesc);
    SetClearColor(0, 0, 0, 1);
    return *this;
}
