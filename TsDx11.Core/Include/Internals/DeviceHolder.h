#pragma once

namespace TS
{
    class DeviceHolder
    {
    public:
        TS::SharedPtr<ID3D11Device>& GetD3D11Device();
        DeviceHolder& SetD3DDevice(TS::SharedPtr<ID3D11Device> pDev);
        DeviceHolder& SetSwapChain(TS::SharedPtr<IDXGISwapChain> pSwapChain);
        TS::SharedPtr<IDXGISwapChain>& GetD3DSwapChain();
        DeviceContext& ImmediateContext();
        DeviceContext& DefferedContext();
        DeviceHolder& Present(unsigned syncInterval = 0, unsigned flags = 0);

        static ManagedArray<unsigned char> LoadCompiledShader(const TsChar* filename);

        VertexShader CreateVertexShader(const Binary& binary) const;
        PixelShader CreatePixelShader(const Binary& binary) const;
        GeometryShader CreateGeometryShader(const Binary& binary) const;
        HullShader CreateHullShader(const Binary& binary) const;
        DomainShader CreateDomainShader(const Binary& binary) const;
        ComputeShader CreateComputeShader(const Binary& binary) const;

        InputLayout CreateInputLayout(Binary& binary, InputElementDesc* desc, unsigned layoutCount) const;
        Buffer CreateBuffer(Binary& binary, D3D11_CPU_ACCESS_FLAG cpuAccessFlag, D3D11_BIND_FLAG bindFlag) const;
        VertexBuffer CreateVertexBuffer(void* const &pVertex , unsigned count , size_t stride) const;
    private:
        TS::SharedPtr<ID3D11Device> _d3d11Device;
        TS::SharedPtr<IDXGISwapChain>_swapChain;
        DeviceContext  _immediateContext;    
        DeviceContext  _defferedContext;      
    };
}
