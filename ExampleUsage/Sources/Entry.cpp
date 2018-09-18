// Dx11SimpleFramework.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "pch.h"
#include "../../TsDx11.Core/Sources/TsDx11Core.h"
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

    Dx11CoreInitializeData initialize;
    {
        initialize.WindowHandle = hwnd;
        initialize.Width        = 1024;
        initialize.Height       = 768;
    }

    GfxSystem core;
    core.Initialize(initialize);
    auto _vertexBuffer = core.Holder().CreateVertexBuffer(points, ARRAYSIZE(points), sizeof(Vector3));

    GfxPipline pipline(core.Holder(),core.ResourceFactory());
    pipline.LoadVertexShader(L"../Debug/VertexShader.cso");
    pipline.LoadPixelShader(L"../Debug/PixelShader.cso");
    pipline.SetupDefault();

    TsString a;

    a = L"a";

    a = a + L"b";

    a += L"a";
    a += L"a";
    a += L"a";

    TsString str(L"abc012abc012abc012");

    str = str.Replace(L"012", L"defg");
    str = str.Replace('a', '*');

    printf("%ls", str.Data());

    auto _true = a.Contain(L"ba");
    auto _false = a.Contain(L"c");

    auto bbb = TsString::Format(L"こんにちは%sおはよう", "aaaaaaaa");
    
    ConstantBffuerFactory cBufferFactor(core.Holder());
    auto cbuffer = cBufferFactor.CreateConstantBuffer<TransformCBuffer>(ShaderType::Pixel);
    MSG tMsg;
    cbuffer.UpdateBuffer(core.Holder().ImmediateContext());
    core.Holder()
        .ImmediateContext()
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
            core.Holder()
                .ImmediateContext()
                .SetVertexBuffer(_vertexBuffer)
                .SetTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
                .Draw(3);

            core.Holder().Present();
        }
    }

    return 0;
}