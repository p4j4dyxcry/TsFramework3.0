#include "pch.h"
#include "TsDx11Core.Internal.h"
namespace TS
{
    GfxSystem::GfxSystem()
    {
    }

    ErrorResult GfxSystem::Initialize(Dx11CoreInitializeData& initializeData)
    {
        ErrorResult error = Error::Success;

        error = InitD3D(initializeData);
        if (error.IsError())
            return error;

        error = SetupDefault(initializeData);
        if (error.IsError())
            return error;

        return error;
    }

    DeviceHolder& GfxSystem::Holder()
    {
        return _device;
    }

    ShaderResourceFactory& GfxSystem::ResourceFactory()
    {
        return _resourceFactory;
    }

    ErrorResult GfxSystem::InitD3D(Dx11CoreInitializeData& initializeData)
    {
        unsigned  devCreateFlag = 0;
#ifdef _DEBUG
        devCreateFlag |= D3D11_CREATE_DEVICE_DEBUG; // D3D11_CREATE_DEVICE_FLAG
#endif
        D3D_FEATURE_LEVEL featureLevels[] ={D3D_FEATURE_LEVEL_11_1,D3D_FEATURE_LEVEL_11_0};
        DXGI_SWAP_CHAIN_DESC swapChainDesc = _makeSwapChainDesc(initializeData.WindowHandle, initializeData.Width, initializeData.Height);

        ID3D11Device*				pDevice = nullptr;
        ID3D11DeviceContext*		pDeviceContext = nullptr;
        IDXGISwapChain*				pSwapChain = nullptr;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, devCreateFlag, featureLevels, 2, D3D11_SDK_VERSION,
            &swapChainDesc, &pSwapChain, &pDevice, nullptr, &pDeviceContext);

        _device
            .SetD3DDevice(_makeD3DShared(pDevice))
            .SetSwapChain(_makeD3DShared(pSwapChain));

        _device.ImmediateContext()
            .SetD3DDeviceContext(_makeD3DShared(pDeviceContext));

        return Error::Make(hr);
    }

    ErrorResult GfxSystem::SetupDefault(Dx11CoreInitializeData& initializeData)
    {
        try
        {
            _resourceFactory.Initialize(_device);

            auto depthStencilTextureDesc= TextureDesc(initializeData.Width, initializeData.Height, DXGI_FORMAT_D24_UNORM_S8_UINT);
            auto depthStencilStateDesc = DepthStencilStateDesc();
            auto rasterizerStateDesc = RasterizerStateDesc(initializeData.Width, initializeData.Height);

            auto rtv = _resourceFactory.CreateBackBufferRenderTarget();
            auto dsv        = _resourceFactory.CreateDepthStencilTarget(depthStencilTextureDesc);
            auto rasterizer = _resourceFactory.CreateRasterizerState(rasterizerStateDesc);
            auto dss        = _resourceFactory.CreateDepthStencilState(depthStencilStateDesc);

            _device.ImmediateContext()
                .SetRenderTarget(rtv)
                .SetDepthStencilTarget(dsv)
                .ApplyBufferTargets()
                .SetRasterizerState(rasterizer)
                .SetDepthStencilState(dss);

        }
        catch (...)
        {
            return Error::Assert(false);
        }

        return Error::Success;
    }
}

