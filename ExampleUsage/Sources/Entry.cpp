﻿// Dx11SimpleFramework.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "pch.h"
#include "../../TsDx11.Core/Include//TsDx11Core.h"
#include "../../TsFramework/Sources/RenderPipline.h"
#include "../../TsFramework/Sources/StructureResources.hlsli"
#include "../../TsFramework/Sources/ConstantBffuerFactory.h"

using namespace TS;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND GenerateWindow(const TS::TsChar * title, unsigned width, unsigned height)
{
    WNDCLASSEX tWndClass;
    const auto hInstance = GetModuleHandle(nullptr);
    tWndClass.cbSize = sizeof(WNDCLASSEX);
    tWndClass.style = CS_HREDRAW | CS_VREDRAW;
    tWndClass.lpfnWndProc = WindowProc;
    tWndClass.cbClsExtra = 0;
    tWndClass.cbWndExtra = 0;
    tWndClass.hInstance = hInstance;
    tWndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    tWndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    tWndClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    tWndClass.lpszMenuName = nullptr;
    tWndClass.lpszClassName = _T("Dx11CoreSample");
    tWndClass.hIconSm = nullptr;

    if (0 == ::RegisterClassEx(&tWndClass))
        return nullptr;

    return CreateWindowEx(
        0
        , tWndClass.lpszClassName
        , title
        , WS_OVERLAPPEDWINDOW
        , CW_USEDEFAULT
        , CW_USEDEFAULT
        , width
        , height
        , nullptr
        , nullptr
        , hInstance
        , nullptr
    );
}

int main()
{    
    const auto hwnd = GenerateWindow(_T("Sample"), 1024, 768);

    if (hwnd == nullptr)
        Error::Assert(false, _T("ウィンドウ生成に失敗しました。"));

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    Vector3 points[]
    {
        Vector3(-0.5f,-0.5f,0.5f),
        Vector3(-0.5f,0.5f,0.5f),
        Vector3(0.5f,-0.5f,0.5f),
        Vector3(0.5f,0.5f,0.5f),
    };

    Dx11CoreInitializeData initialize{};
    {
        initialize.WindowHandle = hwnd;
        initialize.Width        = 1024;
        initialize.Height       = 768;
    }

    GfxSystem core;
    core.Initialize(initialize);

    auto gfxDeviceHolder = core.GetDeviceHolder();

    auto _vertexBuffer = gfxDeviceHolder.CreateVertexBuffer(points, ARRAYSIZE(points), sizeof(Vector3));

    GfxPipline pipline(gfxDeviceHolder,core.ResourceFactory());
    pipline.LoadVertexShader(L"../x64/Debug/VertexShader.cso");
    pipline.LoadPixelShader (L"../x64/Debug/PixelShader.cso");
    pipline.SetupDefault();

    ConstantBffuerFactory cBufferFactor(gfxDeviceHolder);
    auto cbuffer = cBufferFactor.CreateConstantBuffer<TransformCBuffer>(ShaderType::Pixel);
    MSG tMsg;
    cbuffer.UpdateBuffer(gfxDeviceHolder.ImmediateContext());
    gfxDeviceHolder.ImmediateContext()
        .SetConstantBuffer(cbuffer);
    
    while (true)
    {
        if (PeekMessage(&tMsg, nullptr, 0, 0, PM_NOREMOVE))
        {
            if (tMsg.message == WM_QUIT)
                break;
            
            if (GetMessage(&tMsg, nullptr, 0, 0) != -1)
            {
                TranslateMessage(&tMsg);
                DispatchMessage(&tMsg);
            }
        }
        else
        {
            pipline.Apply();
            gfxDeviceHolder.ImmediateContext()
                .SetVertexBuffer(_vertexBuffer)
                .SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
                .Draw(4);

            gfxDeviceHolder.Present();
        }
    }

    return 0;
}