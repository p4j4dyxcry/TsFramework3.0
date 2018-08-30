#include "pch.h"
#include "TsDx11Core.Internal.h"

namespace TS
{
    TsSharedPtr<ID3D11Device>& DeviceHolder::GetD3D11Device()
    {
        Error::Assert(_d3d11Device != nullptr);
        return _d3d11Device;
    }

    DeviceHolder& DeviceHolder::SetD3DDevice(TsSharedPtr<ID3D11Device> pDev)
    {
        _d3d11Device = pDev;
        return *this;
    }

    DeviceHolder& DeviceHolder::SetSwapChain(TsSharedPtr<IDXGISwapChain> pSwapChain)
    {
        _swapChain = pSwapChain;
        return *this;
    }

    TsSharedPtr<IDXGISwapChain>& DeviceHolder::GetD3DSwapChain()
    {
        return _swapChain;
    }
        
    DeviceContext& DeviceHolder::ImmediateContext()
    {
        return _immediateContext;
    }

    DeviceContext& DeviceHolder::DefferedContext()
    {
        return _defferedContext;
    }

    DeviceHolder& DeviceHolder::Present(unsigned syncInterval, unsigned flags)
    {
        Error::Assert(_swapChain != nullptr);
        _swapChain->Present(syncInterval, flags);
        return *this;
    }

    Binary DeviceHolder::LoadCompiledShader(const TsChar* filename)
    {
        return _loadCompiledShader(filename);
    }

    VertexShader DeviceHolder::CreateVertexShader(const Binary& binary) const
    {
        return _createVertexShaderFromMemory(_d3d11Device.get(), binary._data, binary._size);
    }

    PixelShader DeviceHolder::CreatePixelShader(const Binary& binary) const
    {
        return _createPixelShaderFromMemory(_d3d11Device.get(), binary._data, binary._size);
    }

    GeometryShader DeviceHolder::CreateGeometryShader(const Binary& binary) const
    {
        return _createGeometryShaderFromMemory(_d3d11Device.get(), binary._data, binary._size);
    }

    HullShader DeviceHolder::CreateHullShader(const Binary& binary) const
    {
        return _createHullShaderFromMemory(_d3d11Device.get(), binary._data, binary._size);
    }

    DomainShader DeviceHolder::CreateDomainShader(const Binary& binary) const
    {
        return _createDomainShaderFromMemory(_d3d11Device.get(), binary._data, binary._size);
    }

    ComputeShader DeviceHolder::CreateComputeShader(const Binary& binary) const
    {
        return _createComputeShaderFromMemory(_d3d11Device.get(), binary._data, binary._size);
    }

    InputLayout DeviceHolder::CreateInputLayout(Binary& binary, D3D11_INPUT_ELEMENT_DESC* desc, unsigned layoutCount) const
    {
        return _createInputLayout(_d3d11Device.get(), binary._data, binary._size, desc, layoutCount);
    }

    Buffer DeviceHolder::CreateBuffer(Binary& binary, D3D11_CPU_ACCESS_FLAG cpuAccessFlag, D3D11_BIND_FLAG bindFlag) const
    {
        return _createD3DBuffer(_d3d11Device.get(), binary._data, binary._size, cpuAccessFlag, bindFlag);
    }

    VertexBuffer DeviceHolder::CreateVertexBuffer(void* const& pVertex, unsigned count, size_t stride) const
    {
        return VertexBuffer(_d3d11Device.get(), pVertex, count, stride);
    }
}
