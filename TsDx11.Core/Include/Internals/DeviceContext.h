#pragma once

namespace TS
{
    class DeviceContext
    {
    public:
        DeviceContext(TS::SharedPtr<ID3D11DeviceContext> pDeviceContext = nullptr);

        DeviceContext& SetD3DDeviceContext(TS::SharedPtr<ID3D11DeviceContext> pDeviceContext);

        DeviceContext& SetConstantBuffer(IConstantBuffer& constantBuffer);
        DeviceContext& SetIndexBuffer(IndexBufer& buffer);
        DeviceContext& SetVertexBuffer(VertexBuffer& buffer);

        DeviceContext& SetRenderTarget(RenderTarget& _renderTarget , unsigned id = 0);
        DeviceContext& SetDepthStencilTarget(DepthStencilTarget& _depthStencilTarget);

        DeviceContext& ClearColorToRenderTaegets(float r, float g, float b, float a);
        DeviceContext& ClearDepth();
        
        DeviceContext& SetInputLayout(InputLayout&);

        DeviceContext& SetRasterizerState(RasterizerState& rasterizerState);
        DeviceContext& SetDepthStencilState(DepthStencilState& depthStencilState);
        DeviceContext& SetBlendState(BlendState& blendState);
        DeviceContext& SetSamplerState(SamplerState& samplerState , unsigned id,ShaderType shaderType = ShaderType::Pixel);
        DeviceContext& SetTexture(ShaderResourceView& srv , unsigned id, ShaderType shaderType = ShaderType::Pixel);

        DeviceContext& ApplyBufferTargets();
        DeviceContext& SetVertxShader(VertexShader& shader);
        DeviceContext& SetPixelShader(PixelShader& shader);
        DeviceContext& SetGeometryShader(GeometryShader& shader);
        DeviceContext& SetHullShader(HullShader& shader);
        DeviceContext& SetDomainShader(DomainShader& shader);
        DeviceContext& SetComputeShader(ComputeShader& shader);

        DeviceContext& SetTopology(D3D_PRIMITIVE_TOPOLOGY topology);
        D3D_PRIMITIVE_TOPOLOGY GetTopology()const;

        MappedSubResource Map(const ID3DResourcesHolder* d3d11Resource, DeviceContextMapMode mapMode);
        DeviceContext& UnMap(const ID3DResourcesHolder* resourceHolder);

        DeviceContext& Draw(unsigned vertex_count, unsigned start_location = 0);
        DeviceContext& DrawIndexed(unsigned index_count, unsigned start_index_location = 0 ,unsigned base_vertex_location = 0);
    
    private:
        static const unsigned RenderTargetCount = 8;
        RenderTarget _renderTaegets[RenderTargetCount];
        DepthStencilTarget _depthStencilTarget;
        TS::SharedPtr<ID3D11DeviceContext> _deviceContext;
        unsigned _mapCount;
    };
}
