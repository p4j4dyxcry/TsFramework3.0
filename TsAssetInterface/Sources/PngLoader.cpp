#include "pch.h"
#include "PngLoader.h"
#include "structs.h"

//---- GDI+ヘッダ関連
#include <gdiplus.h>
#pragma comment (lib, "gdiplus.lib")
TS::BasicTexture LoadTextureFromFile(const char * fileName)
{
    wchar_t path[MAX_PATH];
    size_t pathLength = 0;

    if (mbstowcs_s(&pathLength, &path[0], MAX_PATH, fileName, _TRUNCATE) != 0)
        return TS::BasicTexture();

    //  GDI+オブジェクト（画像展開に必要）
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    if (GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr) != Gdiplus::Ok)
        return TS::BasicTexture();

    //-------------------------------------------------------------
    // 画像の読み込み
    //-------------------------------------------------------------
    unsigned  width = 0; // 画像の幅
    unsigned  height = 0; //  〃 高さ
    Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(path);
    TS::BasicTexture texture;

    if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok) 
    {
        width = pBitmap->GetWidth();
        height = pBitmap->GetHeight();

        texture.colors = TS::ManagedArray<TS::Color>(width * height);
    }

    //---- 画像イメージの読み込み
    for (unsigned y = 0; y < height; y++)
    {
        for (unsigned x = 0; x < width; x++)
        {
            Gdiplus::Color srcColor;
            pBitmap->GetPixel(x, y, &srcColor);

            auto color = TS::Color();

            color.r = srcColor.GetR();
            color.g = srcColor.GetG();
            color.b = srcColor.GetB();
            color.a = srcColor.GetA();

            texture.colors[y * width + x] = color;
        }
    }
    delete pBitmap;

    //---- GDI+の解放
    Gdiplus::GdiplusShutdown(gdiplusToken);

    return texture;
}