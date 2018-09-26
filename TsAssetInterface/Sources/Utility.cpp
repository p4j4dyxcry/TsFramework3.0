#include "pch.h"
#include "FilePathAnalizer.h"
#include "structs.h"
#include "Utility.h"

#include <gdiplus.h>
#include "ObjParser.h"

#pragma comment (lib, "gdiplus.lib")


namespace TS
{
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
        // ! 画像の読み込み
        //-------------------------------------------------------------
        unsigned  width = 0; // 画像の幅
        unsigned  height = 0; //  〃 高さ

                              // 中身は new している
        Gdiplus::Bitmap* pBitmap = Gdiplus::Bitmap::FromFile(path);
        TS::BasicTexture texture;

        if (pBitmap && pBitmap->GetLastStatus() == Gdiplus::Ok)
        {
            width = pBitmap->GetWidth();
            height = pBitmap->GetHeight();

            texture.colors = TS::Collection<TS::u8Color>(width * height);
        }

        //---- 画像イメージの読み込み
        for (unsigned y = 0; y < height; y++)
        {
            for (unsigned x = 0; x < width; x++)
            {
                Gdiplus::Color srcColor;
                pBitmap->GetPixel(x, y, &srcColor);

                auto color = TS::u8Color();

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

    //! convert obj_model -> common_model
    void build_from_obj(BasicModel& result , const obj_file& raw_data)
    {
        raw_data.meshes.Length();

        result.meshes = Collection<BasicMesh>(raw_data.meshes.Length());

        for(auto raw_data_mesh : raw_data.meshes)
        {
            if (raw_data_mesh.face.Length() == 0)
                continue;
            BasicMesh mesh;
            mesh.postions.AddRange(raw_data.positions);
            mesh.material_name = raw_data_mesh.material_name;
            for(auto raw_data_mesh_face : raw_data_mesh.face)
            {
                Face face;
                
                for(unsigned int i=0; i<3 ; ++i)
                {
                    face.index[i] = raw_data_mesh_face.idx_position[i];

                    if(raw_data_mesh_face.has_normal())
                    {
                        mesh.normals.Resize(raw_data.positions.Length());
                        mesh.normals[face.index[i]] = mesh.normals[raw_data_mesh_face.idx_normal[i]];
                    }

                    if( raw_data_mesh_face.has_uv() )
                    {
                        mesh.uvs.Resize(raw_data.positions.Length());
                        mesh.uvs[face.index[i]] = mesh.uvs[raw_data_mesh_face.idx_texcoord[i]];
                    }
                }
                mesh.indices.Add(face);
            }
            result.meshes.Add(mesh);
        }

        result.materials.Reserve(raw_data.materials.Length());

        for(const auto raw_data_material : raw_data.materials)
        {

            BasicMaterial material;

            material.diffuse[0] = raw_data_material.diffuse.x;
            material.diffuse[1] = raw_data_material.diffuse.y;
            material.diffuse[2] = raw_data_material.diffuse.z;
            material.diffuse[3] = raw_data_material.alpha > 0 ? raw_data_material.alpha : 1.0f;

            material.name = raw_data_material.name;

            material.ambient[0] = raw_data_material.ambient.x;
            material.ambient[1] = raw_data_material.ambient.y;
            material.ambient[2] = raw_data_material.ambient.z;
            material.ambient[3] = 1.0;

            material.specular[0] = raw_data_material.specluer.x;
            material.specular[1] = raw_data_material.specluer.y;
            material.specular[2] = raw_data_material.specluer.z;
            material.specular[3] = raw_data_material.specluerPower;

            material.diffuse_texture  = raw_data_material.diffuse_map;
            material.specular_texture = raw_data_material.specluer_map;
            material.normal_texture   = raw_data_material.bump_map;
            result.materials.Add(material);
        }
    }

    TS::BasicModel LoadModelFromFile(const char* filename)
    {
        FilePathAnalyzer analizer(filename);

        BasicModel result;

        if(analizer.GetExtension() == ".obj")
        {
            ObjParser obj_parser;
            obj_parser.Parse(filename);
            build_from_obj(result, obj_parser.Get());
        }
        
        return result;
    }

}