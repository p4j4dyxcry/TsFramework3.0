﻿#pragma once

namespace TS
{
    ErrorResult _getRefreshRato(unsigned& outNumerator, unsigned& outDenominator, const unsigned width, const unsigned height);

	ManagedArray<unsigned char> _loadCompiledShader(const TsChar* filepath);

    template<class T>
    TS::SharedPtr<T> _makeD3DShared(T*& _ptr) { return TS::SharedPtr<T>(_ptr, ReleaseDeleter<T>()); }

    DXGI_SWAP_CHAIN_DESC _makeSwapChainDesc(HWND hwnd, unsigned w, unsigned h);

    TS::SharedPtr<ID3D11Buffer> _createD3DBuffer(ID3D11Device* device, void * pMemory, size_t pMemorySize, D3D11_CPU_ACCESS_FLAG cpuAccessFlag, D3D11_BIND_FLAG bindFlag);
    TS::SharedPtr<ID3D11Buffer> _createVertexBuffer(ID3D11Device* device, void *pVertex, unsigned vertexCount, size_t stride);
    TS::SharedPtr<ID3D11Buffer> _createIndexBuffer(ID3D11Device* device, void *pMemory, unsigned indexCount);

    template<typename T>
    TS::SharedPtr<ID3D11Buffer> _createConstantBuffer(ID3D11Device * device , T* pMemory)
    {
        return _createD3DBuffer(device, pMemory, sizeof(T), D3D11_CPU_ACCESS_WRITE, D3D11_BIND_CONSTANT_BUFFER);
    }

    VertexShader   _createVertexShaderFromMemory(ID3D11Device * device, void * pCompiledShader, size_t binarySize);
    PixelShader    _createPixelShaderFromMemory(ID3D11Device * device, void * pCompiledShader, size_t binarySize);
    GeometryShader _createGeometryShaderFromMemory(ID3D11Device * device, void * pCompiledShader, size_t binarySize);
    HullShader     _createHullShaderFromMemory(ID3D11Device * device, void * pCompiledShader, size_t binarySize);
    DomainShader   _createDomainShaderFromMemory(ID3D11Device * device, void * pCompiledShader, size_t binarySize);
    ComputeShader  _createComputeShaderFromMemory(ID3D11Device * device, void * pCompiledShader, size_t binarySize);

    InputLayout    _createInputLayout(ID3D11Device* pDevice, void * pCompiledShader, size_t binarySize, InputElementDesc* desc, unsigned layoutCount);

    bool _containShaderType(ShaderType shaderType, ShaderType targetShader);

    D3D11_MAP DeviceContextMapModeToD3D(DeviceContextMapMode d3d11Map);

    //! Desc to hash
    HashCode _getHashCode(const TextureDesc& desc);
    HashCode _getHashCode(const DepthStencilStateDesc& desc);
    HashCode _getHashCode(const RasterizerStateDesc& desc);
    HashCode _getHashCode(const SamplerStateDesc& desc);
    HashCode _getHashCode(const BlendStateDesc& desc);
}
