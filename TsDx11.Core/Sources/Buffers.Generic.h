#pragma once

namespace TS
{
    template< typename T>
    class ConstantBuffer : public IConstantBuffer
    {
    public:
        ConstantBuffer(T& data, ShaderType shaderType, unsigned slot) : IConstantBuffer(TsSharedPtr<ID3D11Buffer>(), shaderType, slot)
        {
            _data = data;
        }

        void GenerateBuffer(ID3D11Device* _device)
        {
            auto buffer = _createD3DBuffer(
                _device,
                &_data,
                sizeof(_data),
                D3D11_CPU_ACCESS_WRITE, D3D11_BIND_CONSTANT_BUFFER);
            _buffer = TsSharedPtr<ID3D11Buffer>(buffer,ReleaseDeleter<ID3D11Buffer>());
        }

        void UpdateBuffer(DeviceContext& deviceContext)
        {
            Error::Assert(_buffer != nullptr, _T("未初期化"));
            auto mappedResource = deviceContext.Map(this, Ts_Map_WriteDiscard);
            memcpy_s(mappedResource.pData, sizeof(_data), _data, sizeof(_data));
            deviceContext.UnMap(this);
        }

        T& Data()
        {
            return _data;;
        };
    private:
        T _data;
    };
}