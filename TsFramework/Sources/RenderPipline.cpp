#include "pch.h"
#include "RenderPipline.h"

TS::GfxPipline::GfxPipline(DeviceHolder& device_context, ShaderResourceFactory& factory)
    :_deviceHolder(device_context),_shaderResourceFactory(factory)
{
}
TS::GfxPipline& TS::GfxPipline::LoadVertexShader(const TsChar* filename)
{
    return *this;
}

TS::GfxPipline& TS::GfxPipline::LoadPixelShader(const TsChar* filename)
{
    return *this;
}
