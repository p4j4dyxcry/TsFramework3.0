#pragma once

namespace TS
{   
    class ConstantBffuerFactory
    {
    public:
        ConstantBffuerFactory(DeviceHolder& holder):_holder(holder){}

        template<typename T>
        ConstantBuffer<T> CreateConstantBuffer(ShaderType type = ShaderType::Unknown)
        {
            T data;
            TS::SharedPtr<ID3D11Buffer> buffer = _createConstantBuffer(_holder.GetD3D11Device().get(), &data);
            return ConstantBuffer<T>(buffer, data, type, data.GetRegisterdIndex());
        }
    private:
        DeviceHolder & _holder;
    };
}