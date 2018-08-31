#pragma once

namespace TS
{
    class GfxPipline
    {
        using Handle = HashCode;
    public:
        GfxPipline(DeviceHolder& holder , ShaderResourceFactory& factory);
        GfxPipline& LoadVertexShader(const TsChar* filename);
        GfxPipline& LoadPixelShader(const TsChar* filename);
        GfxPipline& Apply();
    private:
        DeviceHolder & _deviceHolder;
        ShaderResourceFactory& _shaderResourceFactory;

        VertexShader _vertexShader;
        InputLayout  _inputLayout;
        PixelShader  _pixelShader;
        
        Vector4 _clearColor;

        Handle _rasterizerState;
        Handle _depthStencilState;
        Handle _depthStencilView;
        Handle _blendState;
        Handle _renderTargets[8];
    };
}
