#include "pch.h"
#include "TsDx11Core.Internal.h"

TS::Buffer::Buffer(TS::SharedPtr<ID3D11Buffer> buffer) : _buffer(std::move(buffer))
{
}

TS::SharedPtr<ID3D11Buffer> TS::Buffer::GetD3DBuffer() const
{
    return _buffer;
}

TS::SharedPtr<ID3D11Resource> TS::Buffer::GetD3D11Resource()const
{
    return _buffer;
}

TS::IConstantBuffer::IConstantBuffer(TS::SharedPtr<ID3D11Buffer>& pBuffer, ShaderType shaderType, unsigned slot)
    :Buffer(pBuffer),_shaderType(shaderType),_slot(slot)
{

}

unsigned TS::IConstantBuffer::GetSlot() const
{
    return _slot;
}

TS::ShaderType TS::IConstantBuffer::GetShaderType() const
{
    return _shaderType;
}

TS::VertexBuffer::VertexBuffer(ID3D11Device* device, void* rowData, unsigned dataCount, size_t stride)
    : Buffer(_createVertexBuffer(device, rowData, dataCount, stride)),_size(stride),_dataCount(dataCount)
{
}

unsigned TS::VertexBuffer::GetDataCount() const
{
    return _dataCount;
}

size_t TS::VertexBuffer::GetStride() const
{
    return _size;
}
