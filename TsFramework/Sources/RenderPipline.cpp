#include "pch.h"
#include "RenderPipline.h"

using namespace TS;

TS::GfxPipline::GfxPipline(DeviceHolder& device_context, ShaderResourceFactory& factory)
    :_deviceHolder(device_context),_shaderResourceFactory(factory)
{
}
TS::GfxPipline& TS::GfxPipline::LoadVertexShader(const TsChar* filename)
{
	LocalArray<unsigned char> binary = _loadCompiledShader(filename);
	LocalArray<InputElementDesc> elementDescs = MakeInputLayoutDescFromMemory(binary);
	_vertexShader = _deviceHolder.CreateVertexShader(binary);
	_inputLayout = _deviceHolder.CreateInputLayout(binary, elementDescs, elementDescs._size);
    return *this;
}

TS::GfxPipline& TS::GfxPipline::LoadPixelShader(const TsChar* filename)
{
	LocalArray<unsigned char> binary = _loadCompiledShader(filename);
	_pixelShader = _deviceHolder.CreatePixelShader(binary);
    return *this;
}

GfxPipline & TS::GfxPipline::SetRasterizerState(RasterizerStateDesc & desc)
{
	_shaderResourceFactory.CreateRasterizerState(desc);
	_blendState = _getHashCode(desc);
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
	_depthStencilState = _getHashCode(desc);
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
	_clearColor = { r,g,b,a };
	return *this;
}

GfxPipline & TS::GfxPipline::Apply()
{
	auto& deviceContext = _deviceHolder.ImmediateContext();
	static const Handle invalid_Hash = 0;

	unsigned _index;
	for (auto rt : _renderTargets)
		if(rt != invalid_Hash)
			deviceContext.SetRenderTarget(_shaderResourceFactory.GetRenderTarget(rt), _index++);
	if (_depthStencilView != invalid_Hash)
		deviceContext.SetDepthStencilTarget(_shaderResourceFactory.GetDepthStencilTarget(_depthStencilView));

	deviceContext.ClearColorToRenderTaegets(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
	deviceContext.ClearDepth();

	if (_blendState != invalid_Hash)
		deviceContext.SetBlendState(_shaderResourceFactory.GetBlendState(_blendState));
	if (_rasterizerState != invalid_Hash)
		deviceContext.SetRasterizerState(_shaderResourceFactory.GetRasterizerState(_rasterizerState));
	if (_depthStencilState != invalid_Hash)
		deviceContext.SetDepthStencilState(_shaderResourceFactory.GetDepthStencilState(_depthStencilState));
	
	deviceContext.SetVertxShader(_vertexShader)
				 .SetPixelShader(_pixelShader);

	return *this;
}
