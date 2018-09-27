#pragma once

namespace TS
{
    class Buffer : public ID3DResourcesHolder
    {
    public:
        Buffer(TS::SharedPtr<ID3D11Buffer> buffer);
        TS::SharedPtr<ID3D11Buffer> GetD3DBuffer() const;
        TS::SharedPtr<ID3D11Resource> GetD3D11Resource()const override;
    protected:
        TS::SharedPtr<ID3D11Buffer> _buffer;
    };

    class IConstantBuffer : public Buffer
    {
    public:
        IConstantBuffer(TS::SharedPtr<ID3D11Buffer>& buffer, ShaderType shaderType, unsigned slot);
        unsigned GetSlot() const;
        ShaderType GetShaderType() const;
    private:
        unsigned _slot;
        ShaderType _shaderType;
    };

    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(ID3D11Device* device, void* rowData, unsigned dataCount, size_t stride);
        unsigned GetDataCount() const;
        size_t GetStride() const;
    protected:
        size_t _size;
        unsigned _dataCount;
    };

    using IndexBufer = Buffer;
}