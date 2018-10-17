#include "pch.h"
#include "MqoPaeser.h"
#include "ParserUtil.h"
namespace TS
{
    void build_face(Collection<mqo_face>& target,
        unsigned face_count,
        const Collection<unsigned>& postion_indeces,
        const Collection<float>&    texcoords,
        unsigned material_index,
        bool has_uv,
        bool has_material)
    {
        for (unsigned i = 0; i < face_count - 2; ++i)
        {
            mqo_face f;

            const auto index_0 = postion_indeces[i * 3 + 0];
            const auto index_1 = postion_indeces[i * 3 + 1];
            const auto index_2 = postion_indeces[i * 3 + 2];

            f.index[0] = postion_indeces[index_0];
            f.index[1] = postion_indeces[index_1];
            f.index[2] = postion_indeces[index_2];

            if (has_uv)
            {
                f.uv[0].x = texcoords[index_0 * 2 + 0];
                f.uv[0].y = texcoords[index_0 * 2 + 1];

                f.uv[1].x = texcoords[index_1 * 2 + 2];
                f.uv[1].y = texcoords[index_1 * 2 + 3];

                f.uv[2].x = texcoords[index_2 * 2 + 4];
                f.uv[2].y = texcoords[index_2 * 2 + 5];
            }

            f.has_uv = has_uv;
            f.has_material = has_material;
            f.material_index = material_index;

            target.Add(f);
        }
    }

    Collection<Vector3> read_vertex(FileReader& file_reader)
    {
        Collection<Vector3> result;
        result.Reserve(4096);
        auto line = file_reader.ReadLine();
        while (line.Contain("}") == false)
        {
            result.Add(ReadVector3(line));

            line = file_reader.ReadLine();
        }

        return result;
    }

    Collection<mqo_face> read_faces(FileReader& file_reader)
    {
        Collection<mqo_face> result;
        result.Reserve(4096);
        auto line = file_reader.ReadLine();
        while (line.Contain("}") == false)
        {
            const int face_count = ReadInt(line);
            int material_index = 0;
            Collection<float> texcoords;
            bool has_material = false;
            bool has_texcoords = false;

            SeekNextStringEnd(line, "V(");
            Collection<int> indices = ReadIntArray(line, face_count);

            if (line.Contain("M("))
            {
                SeekNextStringEnd(line, "M(");
                material_index = ReadInt(line);
                has_material = true;
            }

            if (line.Contain("UV("))
            {
                SeekNextStringEnd(line, "UV(");
                texcoords = ReadFloatArray(line, face_count * 2);
                has_texcoords = true;
            }

            build_face(result, 
                face_count,
                ConvertGeometoryToTriangleList(reinterpret_cast<unsigned*>(&indices[0]), face_count),
                texcoords,
                material_index,
                has_texcoords,
                has_material);

            line = file_reader.ReadLine();
        }

        return result;
    }

    mqo_mesh read_object(FileReader& file_reader)
    {
        mqo_mesh mesh;
        mesh.visible = true;

        auto line = file_reader.ReadLine();
        while (line.Contain("}") == false)
        {
            if (line.Contain("visible "))
                mesh.visible = ReadInt(line) != 0;
            else if (line.Contain("color"))
                mesh.color = ReadVector3(line);
            else if (line.Contain("vertex"))
                mesh.postions = read_vertex(file_reader);
            else if (line.Contain("face"))
                mesh.faces = read_faces(file_reader);

            line = file_reader.ReadLine();
        }

        return mesh;
    }

    Collection<mqo_material> read_material(FileReader& file_reader)
    {
        Collection<mqo_material> materials;

        auto line = file_reader.ReadLine();
        while (line.Contain("}") == false)
        {
            mqo_material material;

            if(line.Contain("col("))
            {
                SeekNextStringEnd(line, "col(");
                material.color = ReadVector3(line);
            }

            if (line.Contain("dif("))
            {
                SeekNextStringEnd(line, "dif(");
                material.diffuse = ReadFloat(line);
            }

            if (line.Contain("amb("))
            {
                SeekNextStringEnd(line, "amb(");
                material.ambient = ReadFloat(line);
            }

            if (line.Contain("emi("))
            {
                SeekNextStringEnd(line, "emi(");
                material.emissib = ReadFloat(line);
            }

            if (line.Contain("spc("))
            {
                SeekNextStringEnd(line, "spc(");
                material.specular = ReadFloat(line);
            }

            if (line.Contain("power("))
            {
                SeekNextStringEnd(line, "power(");
                material.specular_power = ReadFloat(line);
            }

            if (line.Contain("tex("))
            {
                SeekNextStringEnd(line, "tex(");
                material.texture_name = ReadStringWithDoubleQuotation(line);
            }
            
            materials.Add(material);
            line = file_reader.ReadLine();
        }

        return materials;
    }



    bool TS::MqoPaeser::Parse(const char * filepath)
    {
        ManagedArray<unsigned char> binary = ReadBinary(filepath);

        FileReader stream(binary);

        _file = mqo_file();

        while (stream.Eof() == false)
        {
            auto&& line = stream.ReadLine();

            if (FirstOf(line.ToArray(), "Material "))
                _file.materials = read_material(stream);
            else if (FirstOf(line.ToArray(), "Object  "))
                _file.meshes.Add(read_object(stream));
        }

        return true;
    }

    bool MqoPaeser::SaveAs(const char* filepath)
    {
        (void)filepath; // un used
        return false;
    }
}

