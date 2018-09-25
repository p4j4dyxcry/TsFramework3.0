#pragma once

namespace TS
{
    using NameString = char[256];
    struct Color
    {
        union
        {
            unsigned char rgba[4]{};
            struct
            {
                unsigned char r, g, b, a;
            };
        };

    };

    struct Face
    {
        unsigned index[3]{};
    };

    struct BasicMaterial
    {
        NameString  name{};
        float diffuse[4]{};
        float specular[4]{};

        HashCode diffuse_texture{};
        HashCode specular_texture{};
        HashCode normal_texture{};
    };

    struct BasicMesh
    {              
        NameString  name{};
        ManagedArray<Vector3> postions{};
        ManagedArray<Vector3> normals{};
        ManagedArray<Vector3> tangents{};
        ManagedArray<Vector3> binormals{};
        ManagedArray<Vector3> colors{};
        ManagedArray<Vector2> uvs{};
        ManagedArray<Face>    indices{};

        HashCode material{};
    };

    struct BasicTexture
    {
        NameString  name{};
        unsigned    width{};
        unsigned    height{};

        ManagedArray<Color> colors{};
    };

    struct BasicMeshTransform
    {
        BasicMesh * pMesh{};
        BasicMesh * pParent{};
        BasicMesh * pSubling{};

        Vector3     translate{};
        Vector3     scale{};
        Vector3     rotate{};
    };
    
    struct BasicModel
    {
        ManagedArray<BasicMesh>          meshes;
        ManagedArray<BasicMeshTransform> transforms;
        ManagedArray<BasicMaterial>      materials;
    };

    TS::BasicTexture LoadTextureFromFile(const char * fileName);
}