#pragma once

namespace TS
{
    template< typename T>
    class ConstantBuffer : public IConstantBuffer
    {
    public:
        ConstantBuffer(TS::SharedPtr<ID3D11Buffer> buffer,T& data, ShaderType shaderType, unsigned slot) : IConstantBuffer(buffer, shaderType, slot)
        {
            _data = data;
        }

        void UpdateBuffer(DeviceContext& deviceContext)
        {
            Error::Assert(_buffer != nullptr, _T("未初期化"));
            auto mappedResource = deviceContext.Map(this, Ts_Map_WriteDiscard);
            memcpy_s(mappedResource.pData, sizeof(T), &_data, sizeof(T));
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