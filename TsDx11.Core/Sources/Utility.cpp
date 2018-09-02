#include "pch.h"
#include <fstream>
#include "TsDx11Core.Internal.h"


TS::ErrorResult TS::_getRefreshRato(unsigned& outNumerator, unsigned& outDenominator, const unsigned width, const unsigned height)
{
    outNumerator = 1;
    outDenominator = 60;

    HRESULT hr = S_OK;
    IDXGIFactory* factory; // factory
    IDXGIAdapter* adapter; // videocard
    IDXGIOutput* adapterOutput; // monitor
    UINT numModes;

    // Create a DirectX graphics interface factory
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    auto erroData = Error::Make(hr);
    if (erroData.IsError())
    {
        SAFE_RELEASE(factory);
        return erroData;
    }

    // use the factory to create an adpter for the primary graphics interface(video card)
    hr = factory->EnumAdapters(0, &adapter);
    erroData = Error::Make(hr);
    if (erroData.IsError())
    {
        SAFE_RELEASE(adapter);
        SAFE_RELEASE(factory);
        return erroData;
    }

    // enumerrate primary adapter output(monitor)
    hr = adapter->EnumOutputs(0, &adapterOutput);
    erroData = Error::Make(hr);
    if (erroData.IsError())
    {
        SAFE_RELEASE(adapterOutput);
        SAFE_RELEASE(adapter);
        SAFE_RELEASE(factory);
        return erroData;
    }

    // get the number of modes that fit the DXGI_FORMAT_R8G8B8_UNORM display format forthe adapter output(monitor)
    hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);

    erroData = Error::Make(hr);
    if (erroData.IsError())
    {
        SAFE_RELEASE(adapterOutput);
        SAFE_RELEASE(adapter);
        SAFE_RELEASE(factory);
        return erroData;
    }

    // create alist to hold all possible display modes for this monitor/video card combination
    auto displayModeList = new (DXGI_MODE_DESC[numModes]);

    // now fill the display mode list structures
    hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes,
        displayModeList);
    erroData = Error::Make(hr);
    if (erroData.IsError())
    {
        SAFE_DELETE(displayModeList);
        SAFE_RELEASE(adapterOutput);
        SAFE_RELEASE(adapter);
        SAFE_RELEASE(factory);
        return erroData;
    }

    // now go through all the display modes and find the one that matches the screen width and height
    // when a match is found store the numerator and denominator of the refresh rate for that monitor
    for (unsigned i = 0; i < numModes; i++)
    {
        if (displayModeList[i].Width == width)
        {
            if (displayModeList[i].Height == height)
            {
                outNumerator = displayModeList[i].RefreshRate.Numerator;
                outDenominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    SAFE_DELETE(displayModeList);
    SAFE_RELEASE(adapterOutput);
    SAFE_RELEASE(adapter);
    SAFE_RELEASE(factory);

    return Error::Success;
}

DXGI_SWAP_CHAIN_DESC TS::_makeSwapChainDesc(HWND hwnd, unsigned w, unsigned h)
{
    DXGI_SWAP_CHAIN_DESC swapchainDesc;
    {
        swapchainDesc.BufferCount = 2;
        swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        swapchainDesc.OutputWindow = hwnd;
        swapchainDesc.SampleDesc.Count = 1;
        swapchainDesc.SampleDesc.Quality = 0;
        swapchainDesc.Windowed = true;
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        {
            swapchainDesc.BufferDesc.Width = w;
            swapchainDesc.BufferDesc.Height = h;
            swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
            swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
            swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
            swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
        }
    }
    return swapchainDesc;
}

TsSharedPtr<ID3D11Buffer> TS::_createD3DBuffer(ID3D11Device * device, void * pMemory, size_t pMemorySize, D3D11_CPU_ACCESS_FLAG cpuAccessFlag, D3D11_BIND_FLAG bindFlag)
{
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.ByteWidth = (unsigned )pMemorySize;
    bufferDesc.BindFlags = bindFlag;
    bufferDesc.Usage =
        (cpuAccessFlag & D3D11_CPU_ACCESS_WRITE) ?
        D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    bufferDesc.CPUAccessFlags = cpuAccessFlag;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA resource;
    resource.pSysMem = pMemory;
    resource.SysMemPitch = 0;
    resource.SysMemSlicePitch = 0;

    ID3D11Buffer* pBuffer;
    HRESULT hr = device->CreateBuffer(&bufferDesc, &resource, &pBuffer);
    Error::Make(hr).DoError(true);

    return _makeD3DShared(pBuffer);
}

TsSharedPtr<ID3D11Buffer> TS::_createVertexBuffer(ID3D11Device* device, void* pVertex, unsigned vertexCount, size_t stride)
{
    return _createD3DBuffer(device, pVertex, stride * vertexCount, static_cast<D3D11_CPU_ACCESS_FLAG>(0), D3D11_BIND_VERTEX_BUFFER);
}

TsSharedPtr<ID3D11Buffer> TS::_createIndexBuffer(ID3D11Device* device, void* pMemory, unsigned indexCount)
{
    return _createD3DBuffer(device, pMemory, sizeof(unsigned) * indexCount, static_cast<D3D11_CPU_ACCESS_FLAG>(0), D3D11_BIND_INDEX_BUFFER);
}

TS::VertexShader TS::_createVertexShaderFromMemory(ID3D11Device* device, void* pCompiledShader, size_t binarySize)
{
    ID3D11VertexShader * pShader;
    HRESULT hr = device->CreateVertexShader(pCompiledShader, binarySize, nullptr, &pShader);
    Error::Make(hr).DoError(true);
    return _makeD3DShared(pShader);
}

TS::PixelShader TS::_createPixelShaderFromMemory(ID3D11Device * device, void * pCompiledShader, size_t binarySize)
{
    ID3D11PixelShader * pShader;
    HRESULT hr = device->CreatePixelShader(pCompiledShader, binarySize, nullptr, &pShader);
    Error::Make(hr).DoError(true);
    return _makeD3DShared(pShader);
}

TS::GeometryShader TS::_createGeometryShaderFromMemory(ID3D11Device* device, void* pCompiledShader, size_t binarySize)
{
    ID3D11GeometryShader * pShader;
    HRESULT hr = device->CreateGeometryShader(pCompiledShader, binarySize, nullptr, &pShader);
    Error::Make(hr).DoError(true);
    return _makeD3DShared(pShader);
}

TS::HullShader TS::_createHullShaderFromMemory(ID3D11Device* device, void* pCompiledShader, size_t binarySize)
{
    ID3D11HullShader * pShader;
    HRESULT hr = device->CreateHullShader(pCompiledShader, binarySize, nullptr, &pShader);
    Error::Make(hr).DoError(true);
    return _makeD3DShared(pShader);
}

TS::DomainShader TS::_createDomainShaderFromMemory(ID3D11Device* device, void* pCompiledShader, size_t binarySize)
{
    ID3D11DomainShader * pShader;
    HRESULT hr = device->CreateDomainShader(pCompiledShader, binarySize, nullptr, &pShader);
    Error::Make(hr).DoError(true);
    return _makeD3DShared(pShader);
}

TS::ComputeShader TS::_createComputeShaderFromMemory(ID3D11Device* device, void* pCompiledShader, size_t binarySize)
{
    ID3D11ComputeShader * pShader;
    HRESULT hr = device->CreateComputeShader(pCompiledShader, binarySize, nullptr, &pShader);
    Error::Make(hr).DoError(true);
    return _makeD3DShared(pShader);
}

TS::InputLayout TS::_createInputLayout(ID3D11Device* pDevice,
                                                     void * pCompiledShader,
                                                     size_t binarySize,
                                                     InputElementDesc* desc ,
                                                     unsigned layoutCount)
{
    ID3D11InputLayout * pInputLayout;
    HRESULT hr = pDevice->CreateInputLayout(desc,
        layoutCount,
        pCompiledShader,
        binarySize,
        &pInputLayout);
    Error::Make(hr).DoError(true);
    return _makeD3DShared(pInputLayout);

}


TS::Binary TS::_loadCompiledShader(const TsChar* filepath )
{
    std::ifstream ifs(filepath, std::ifstream::in | std::ifstream::binary);

    if (ifs.fail())
    {
        Error::Make(_T("File Read error")).Log();
        return {nullptr, 0};
    }

    const unsigned begin = static_cast<unsigned>(ifs.tellg());
    ifs.seekg(0, ifs.end);

    const unsigned end = static_cast<unsigned>(ifs.tellg());
    size_t binarySize = (end - begin);
    ifs.clear();
    ifs.seekg(0, ifs.beg);
    void* pCompiledShader = new(unsigned char[binarySize]);
    ifs.read(static_cast<char*>(pCompiledShader), binarySize);

    return {static_cast<unsigned char*>(pCompiledShader),binarySize};
}

bool TS::_containShaderType(ShaderType shaderType, ShaderType targetShader)
{
    return shaderType & targetShader;
}

D3D11_MAP TS::DeviceContextMapModeToD3D(DeviceContextMapMode d3d11Map)
{
    return static_cast<D3D11_MAP>(d3d11Map);
}

TS::HashCode TS::_getHashCode(const TextureDesc& desc)
{
    HashCode hash = desc.Width * 0x914;
    hash += desc.Height * 0x7f8;
    hash += desc.CpuAccessFlag * 0x8000;
    hash += static_cast<HashCode>(desc.Format) * 0xf10400;
    hash += desc.Id;
    return hash;
}

TS::HashCode TS::_getHashCode(const DepthStencilStateDesc& desc)
{
    HashCode hash = desc.ZWriteEnable ? 0xf00 : 0;
    hash += desc.ZEnable ? 0xf0 : 0;
    hash += static_cast<HashCode>(desc.Compare) * 0x0f;
    hash += desc.Id;
    return hash;
}

TS::HashCode TS::_getHashCode(const RasterizerStateDesc& desc)
{
    HashCode hash = static_cast<HashCode>(desc.Culling) * 0xf00000;
    hash += static_cast<HashCode>(desc.Fill) * 0xf0000;
    hash += static_cast<HashCode>(desc.Viewport.Width) * 0x7361;
    hash += static_cast<HashCode>(desc.Viewport.Height) * 0x1366f;
    hash += static_cast<HashCode>(desc.Viewport.MaxDepth) * 0x45154;
    hash += static_cast<HashCode>(desc.Viewport.MinDepth) * 0x24114;
    hash += static_cast<HashCode>(desc.Viewport.TopLeftX) * 0x86458;
    hash += static_cast<HashCode>(desc.Viewport.TopLeftY) * 0x117117;
    hash += desc.UseAntiAlias ? 1 : 0;
    return hash;
}

TS::HashCode TS::_getHashCode(const SamplerStateDesc& desc)
{
    HashCode hash = 0;
    hash += static_cast<HashCode>(desc.Filter) * 0x7527614;
    hash += static_cast<HashCode>(desc.AddressU) * 0x4422508a;
    hash += static_cast<HashCode>(desc.AddressV) * 0xba42125;
    hash += static_cast<HashCode>(desc.AddressW) * 0x325ffa43;
    hash += static_cast<HashCode>(desc.MipLODBias) * 0xf101f7d;
    hash += static_cast<HashCode>(desc.MaxAnisotropy) * 0xdd242;
    hash += static_cast<HashCode>(desc.Compare) * 0xf1462;
    hash += static_cast<HashCode>(desc.BorderColor[0]) * 0x166629f;
    hash += static_cast<HashCode>(desc.BorderColor[1]) * 0x296213;
    hash += static_cast<HashCode>(desc.BorderColor[2]) * 0x1ff653;
    hash += static_cast<HashCode>(desc.BorderColor[3]) * 0xbb31677;
    hash += static_cast<HashCode>(desc.MinLOD) * 0x3211f87;
    hash += static_cast<HashCode>(desc.MaxLOD) * 0x8851a0a0;

    return hash;
}

TS::HashCode TS::_getHashCode(const BlendStateDesc& desc)
{
    return static_cast<HashCode>(desc.Mode);
}


