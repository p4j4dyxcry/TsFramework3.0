#pragma once
#include "I3DModelParser.h"
#include "FileReader.h"

namespace TS
{
    // mqo Material
    struct mqo_material
    {
        Vector4 color;
        float diffuse;
        float ambient;
        float emissib;
        float specular;
        float specular_power;
        StringA texture_name;
    };

    // mqo vertex
    struct mqo_vertex
    {
        Vector3 pos;
        Vector3 normal;
        Vector2 uv;
    };

    // mqo face
    struct mqo_face
    {
        using int3 = int[3];
        int3 index;
        
        bool has_material;
        int  material_index;

        bool has_uv;
        Vector2 uv[3];      
    };

    struct mqo_mesh
    {
        Collection<mqo_face>   faces;
        Collection<Vector3>    postions;

        bool visible;
        Vector3 color;
    };

    struct mqo_file
    {
        Collection<mqo_mesh> meshes;
        Collection<mqo_material> materials;
    };

    class MqoPaeser : public TS::I3ModelParser
    {
    public:
        bool Parse(const char * filepath)override;
        bool SaveAs(const char * filepath)override;

    private:
        mqo_file _file;
    };
}
