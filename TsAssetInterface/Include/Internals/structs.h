#pragma once

namespace TS
{
    struct u8Color
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
        StringA  name{};
        float diffuse [4]{};
        float specular[4]{};
        float ambient [4]{};

        StringA diffuse_texture{};
        StringA specular_texture{};
        StringA normal_texture{};
    };

    struct BasicMesh
    {              
        StringA  name{};
        Collection<Vector3> postions{};
        Collection<Vector3> normals{};
        Collection<Vector3> colors{};
        Collection<Vector2> uvs{};
        Collection<Face>    indices{};

        StringA material_name{};
    };

    struct BasicTexture
    {
        StringA             name{};
        unsigned            width{};
        unsigned            height{};

        Collection<u8Color> colors{};
    };

    struct BasicMeshTransform
    {
        BasicMesh * pMesh   {};
        BasicMesh * pParent {};
        BasicMesh * pSubling{};

        Vector3     translate{};
        Vector3     scale    {};
        Vector3     rotate   {};
    };
    
    struct BasicModel
    {
        Collection<BasicMesh>          meshes;
        Collection<BasicMeshTransform> transforms;
        Collection<BasicMaterial>      materials;
    };
}