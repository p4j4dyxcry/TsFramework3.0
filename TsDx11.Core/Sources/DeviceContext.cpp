#include "pch.h"
#include "TsDx11Core.Internal.h"

TS::DeviceContext::DeviceContext(TsSharedPtr<ID3D11DeviceContext> id3_d11_device_context)
    : _deviceContext(id3_d11_device_context),_mapCount(0)
{
}

TS::DeviceContext& TS::DeviceContext::SetD3DDeviceContext(TsSharedPtr<ID3D11DeviceContext> pDeviceContext)
{
    _deviceContext = pDeviceContext;
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetConstantBuffer(IConstantBuffer& constantBuffer)
{
    Error::Assert(_deviceContext != nullptr);
    const auto shaderType = constantBuffer.GetShaderType();
    const auto slot = constantBuffer.GetSlot();
    auto buffer = constantBuffer.GetD3DBuffer().get();
    if (_containShaderType(shaderType, Ts_Vertex))
        _deviceContext->VSSetConstantBuffers(slot, 1, &buffer);
    if (_containShaderType(shaderType, Ts_Pixel))
        _deviceContext->PSSetConstantBuffers(slot, 1, &buffer);
    if (_containShaderType(shaderType, Ts_Geometry))
        _deviceContext->GSSetConstantBuffers(slot, 1, &buffer);
    if (_containShaderType(shaderType, Ts_Hull))
        _deviceContext->HSSetConstantBuffers(slot, 1, &buffer);
    if (_containShaderType(shaderType, Ts_Domain))
        _deviceContext->DSSetConstantBuffers(slot, 1, &buffer);
    if (_containShaderType(shaderType, Ts_Compute))
        _deviceContext->CSSetConstantBuffers(slot, 1, &buffer);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetIndexBuffer(IndexBufer& buffer)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->IASetIndexBuffer(buffer.GetD3DBuffer().get(), DXGI_FORMAT_R32_UINT, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetVertexBuffer(VertexBuffer& vertexBuffer)
{
    Error::Assert(_deviceContext != nullptr);
    const unsigned ofset = 0;
    const unsigned stride = (unsigned)vertexBuffer.GetStride();
    ID3D11Buffer* buffer = vertexBuffer.GetD3DBuffer().get();
    _deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &ofset);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetRenderTarget(RenderTarget& _renderTarget, unsigned id)
{
    _renderTaegets[id] = _renderTarget;
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetDepthStencilTarget(DepthStencilTarget& target)
{
    _depthStencilTarget = target;
    return *this;
}

TS::DeviceContext& TS::DeviceContext::ClearColorToRenderTaegets(float r, float g, float b, float a)
{
    Error::Assert(_deviceContext != nullptr);
    float colors[4] = { r,g,b,a };
    for (int i = 0; i < RenderTargetCount; ++i)
    {
        auto rtv = _renderTaegets[i].GetD3DRenderTargetView().get();
        if ( rtv != nullptr)
            _deviceContext->ClearRenderTargetView(rtv, colors);
    }
    return *this;
}

TS::DeviceContext& TS::DeviceContext::ClearDepth()
{
    Error::Assert(_deviceContext != nullptr);
    auto dsv = _depthStencilTarget.GetD3DDepthStencilView().get();
    if (dsv != nullptr)
        _deviceContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetInputLayout(InputLayout& inputLayout)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->IASetInputLayout(inputLayout.Get().get());
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetRasterizerState(RasterizerState& rasterizerState)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->RSSetState(rasterizerState.GetD3DRasterizerState().get());
    _deviceContext->RSSetViewports(1, &rasterizerState.ViewPort());
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetDepthStencilState(DepthStencilState& depthStencilState)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->OMSetDepthStencilState(depthStencilState.GetDepthStencilState().get(),0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetBlendState(BlendState& blendState)
{
    Error::Assert(_deviceContext != nullptr);
    float blendFactor[4] = { 0.0f , 0.0f , 0.0f , 0.0f };
    _deviceContext->OMSetBlendState(blendState.GetD3DBlendState().get(), blendFactor, 0xffffff);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetSamplerState(SamplerState& samplerState, unsigned id, ShaderType shaderType)
{
    Error::Assert(_deviceContext != nullptr);

    auto pSamler = samplerState.GetD3DBlendState().get();
    if (_containShaderType(shaderType, Ts_Vertex))
        _deviceContext->VSSetSamplers(id, 1, &pSamler);
    if (_containShaderType(shaderType, Ts_Pixel))
        _deviceContext->PSSetSamplers(id, 1, &pSamler);
    if (_containShaderType(shaderType, Ts_Geometry))
        _deviceContext->GSSetSamplers(id, 1, &pSamler);
    if (_containShaderType(shaderType, Ts_Hull))
        _deviceContext->HSSetSamplers(id, 1, &pSamler);
    if (_containShaderType(shaderType, Ts_Domain))
        _deviceContext->DSSetSamplers(id, 1, &pSamler);
    if (_containShaderType(shaderType, Ts_Compute))
        _deviceContext->CSSetSamplers(id, 1, &pSamler);

    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetTexture(ShaderResourceView& srv, unsigned id, ShaderType shaderType)
{
    Error::Assert(_deviceContext != nullptr);

    if (_containShaderType(shaderType, Ts_Vertex))
        _deviceContext->VSSetShaderResources(id, 1 ,&srv.Srv);
    if (_containShaderType(shaderType, Ts_Pixel))
        _deviceContext->PSSetShaderResources(id, 1, &srv.Srv);
    if (_containShaderType(shaderType, Ts_Geometry))
        _deviceContext->GSSetShaderResources(id, 1, &srv.Srv);
    if (_containShaderType(shaderType, Ts_Hull))
        _deviceContext->HSSetShaderResources(id, 1, &srv.Srv);
    if (_containShaderType(shaderType, Ts_Domain))
        _deviceContext->DSSetShaderResources(id, 1, &srv.Srv);
    if (_containShaderType(shaderType, Ts_Compute))
        _deviceContext->CSSetShaderResources(id, 1, &srv.Srv);

    return *this;
}

TS::DeviceContext& TS::DeviceContext::ApplyBufferTargets()
{
    Error::Assert(_deviceContext != nullptr);

    ID3D11RenderTargetView * renderTargets[RenderTargetCount]{};
    ID3D11DepthStencilView * dsv = _depthStencilTarget.GetD3DDepthStencilView().get();

    int rtNum = 0;
    for (; rtNum < RenderTargetCount; ++rtNum)
    {
        const auto rtv = _renderTaegets[rtNum].GetD3DRenderTargetView();
        if (rtv == nullptr)
            break;
        renderTargets[rtNum] = rtv.get();
    }

    if (rtNum == 0)
        Error::Assert(false, _T("レンダーターゲット未設定"));

    _deviceContext->OMSetRenderTargets(rtNum, renderTargets, dsv);

    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetVertxShader(VertexShader& shader)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->VSSetShader(shader.Get().get(), nullptr, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetPixelShader(PixelShader& shader)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->PSSetShader(shader.Get().get(), nullptr, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetGeometryShader(GeometryShader& shader)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->GSSetShader(shader.Get().get(), nullptr, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetHullShader(HullShader& shader)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->HSSetShader(shader.Get().get(), nullptr, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetDomainShader(DomainShader& shader)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->DSSetShader(shader.Get().get(), nullptr, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetComputeShader(ComputeShader& shader)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->CSSetShader(shader.Get().get(), nullptr, 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::SetTopology(D3D_PRIMITIVE_TOPOLOGY topology)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->IASetPrimitiveTopology(topology);
    return *this;
}

D3D_PRIMITIVE_TOPOLOGY TS::DeviceContext::GetTopology() const
{
    Error::Assert(_deviceContext != nullptr);
    D3D_PRIMITIVE_TOPOLOGY _;
    _deviceContext->IAGetPrimitiveTopology(&_);
    return _;
}

TS::MappedSubResource TS::DeviceContext::Map(const ID3DResourcesHolder* d3d11Resource , DeviceContextMapMode mapMode)
{
    Error::Assert(_deviceContext != nullptr);
    Error::Assert(++_mapCount == 1);
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = _deviceContext->Map(d3d11Resource->GetD3D11Resource().get(), 0, DeviceContextMapModeToD3D(mapMode), 0, &mappedResource);
    Error::Make(hr).DoError(true);

    return { mappedResource.pData,mappedResource.RowPitch,mappedResource.DepthPitch };
}

TS::DeviceContext& TS::DeviceContext::UnMap(const ID3DResourcesHolder* resourceHolder)
{
    Error::Assert(_deviceContext != nullptr);
    Error::Assert(--_mapCount == 0);
    _deviceContext->Unmap(resourceHolder->GetD3D11Resource().get(), 0);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::Draw(unsigned vertex_count, unsigned start_location)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->Draw(vertex_count, start_location);
    return *this;
}

TS::DeviceContext& TS::DeviceContext::DrawIndexed(unsigned index_count, unsigned start_index_location,
    unsigned base_vertex_location)
{
    Error::Assert(_deviceContext != nullptr);
    _deviceContext->DrawIndexed(index_count, start_index_location,base_vertex_location);
    return *this;
}
