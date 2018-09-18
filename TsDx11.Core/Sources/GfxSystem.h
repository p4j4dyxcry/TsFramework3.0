#pragma once


namespace TS
{
    struct Dx11CoreInitializeData
    {
        HWND WindowHandle;
        unsigned Width;
        unsigned Height;
    };

    class GfxSystem
    {
    public:
        GfxSystem();
        ErrorResult Initialize(Dx11CoreInitializeData& initializeData);
        DeviceHolder& Holder();
        ShaderResourceFactory& ResourceFactory();
    private:
        DeviceHolder _device;
        ShaderResourceFactory _resourceFactory;
    private:
        ErrorResult InitD3D(Dx11CoreInitializeData& initializeData);
        ErrorResult SetupDefault();
    private:
        TS_CLASS_DISABLE_COPY(GfxSystem);
        TS_CLASS_DISABLE_MOVE(GfxSystem);
    };
}