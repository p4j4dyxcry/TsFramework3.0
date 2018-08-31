// Dx11SimpleFramework.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "pch.h"
#include "../../TsDx11.Core/Sources/TsDx11Core.h"

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
    
#ifdef _DEBUG
    auto vertexShaderBinary = DeviceHolder::LoadCompiledShader(_T("../Debug/VertexShader.cso"));
    auto pixelShaderBinary = DeviceHolder::LoadCompiledShader(_T("../Debug/PixelShader.cso"));
#else
    auto vertexShaderBinary = DeviceHolder::LoadCompiledShader(_T("../Release/VertexShader.cso"));
    auto pixelShaderBinary = DeviceHolder::LoadCompiledShader(_T("../Release/PixelShader.cso"));
#endif
    Vector3 points[]
    {
        Vector3(-0.5f,-0.5f,0.5f),
        Vector3(-0.5f,0.5f,0.5f),
        Vector3(0.5f,-0.5f,0.5f),
        Vector3(0.5f,0.5f,0.5f),
    };

    InputElementDesc desc[] =
    {
        ElementDescPosition3f(),
    };

    MakeInputLayoutDescFromMemory(vertexShaderBinary).Delete();

    Dx11CoreInitializeData initialize;
    {
        initialize.WindowHandle = hwnd;
        initialize.Width = 1024;
        initialize.Height = 768;
    }

    GfxSystem core;
    core.Initialize(initialize);
    auto holder = core.Holder();

    auto vertexShader = holder.CreateVertexShader(vertexShaderBinary);
    auto pixelShader  = holder.CreatePixelShader(pixelShaderBinary);

    auto inputLayout  = holder.CreateInputLayout(vertexShaderBinary, desc, 1);
    auto vertexBuffer = holder.CreateVertexBuffer(&points, ARRAYSIZE(points), sizeof(Vector3));

    holder.ImmediateContext()
        .SetVertxShader(vertexShader)
        .SetPixelShader(pixelShader)
        .SetVertexBuffer(vertexBuffer)
        .SetInputLayout(inputLayout)
        .SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    MSG tMsg;

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
            holder
                .ImmediateContext()
                .ClearColorToRenderTaegets(.5, .5, 0, 1)
                .ClearDepth()
                .Draw(4);

            core.Holder().Present();
        }
    }

    return 0;
}