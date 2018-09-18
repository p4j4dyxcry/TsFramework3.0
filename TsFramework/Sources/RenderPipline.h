#pragma once

namespace TS
{
    class GfxPipline
    {
        using Handle = HashCode;
    public:
        GfxPipline(DeviceHolder& holder ,ShaderResourceFactory& factory);
        GfxPipline& LoadVertexShader(const TsChar* filename);
        GfxPipline& LoadPixelShader(const TsChar* filename);
        GfxPipline& SetRasterizerState(RasterizerStateDesc& desc);
        GfxPipline& SetDepthStencilState(DepthStencilStateDesc &desc);
        GfxPipline& SetBlendState(BlendStateDesc& desc);
        GfxPipline& SetRenderTarget(TextureDesc& desc , int slot);
        GfxPipline& SetDepthStencilTarget(TextureDesc& desc);
        GfxPipline& SetClearColor(float r , float g , float b , float a = 0);
        GfxPipline& Apply();
        GfxPipline& SetupDefault();
    private:
        DeviceHolder& _deviceHolder;
        ShaderResourceFactory& _shaderResourceFactory;

        VertexShader _vertexShader;
        InputLayout  _inputLayout;
        PixelShader  _pixelShader;

        Vector4 _clearColor;

        Handle _rasterizerState;
        Handle _depthStencilState;
        Handle _depthStencilView;
        Handle _blendState;
        Handle _renderTargets[8]{};
    };
}
