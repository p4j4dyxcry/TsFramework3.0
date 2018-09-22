#include "pch.h"
#include "FileReader.h"
#include "Path.h"
#include "Directory.h"
#include "FilePathAnalizer.h"
#include "structs.h"
#include "I3DModelParser.h"
#include "ObjParser.h"
#include <fstream>

namespace TS 
{
	bool FirstOf(const Binary& binary, const char * tag)
	{
		unsigned index = 0;
		while (tag != '\0')
		{
			if (binary[index] != (unsigned char)(tag[index]))
				return false;
			index++;
		}
		return true;
	}

	void SeekNextWhiteSpace(FileReader& reader)
	{
		while (reader.Read<char>() != ' '){}
	}

	float ReadFloat(FileReader& source)
	{
		float result;
		SeekNextWhiteSpace(source);
		const char * data = (const char *)(source.ToArray().Data());
		sscanf_s(data,
			"%f",
			&result);
		return result;
	};
	   
	Vector2 ReadVector2(FileReader& source)
	{
		Vector2 vector;
		SeekNextWhiteSpace(source);
		const char * data = (const char *)(source.ToArray().Data());
		sscanf_s(data,
			"%f %f",
			&vector.x,
			&vector.y);
		return vector;
	};

	Vector3 ReadVector3(FileReader& source)
	{
		Vector3 vector;
		SeekNextWhiteSpace(source);
		const char * data = (const char *)(source.ToArray().Data());
		sscanf_s(data,
			"%f %f",
			&vector.x,
			&vector.y,
			&vector.z);
		return vector;
	};

	StringA ReadString(FileReader& reader)
	{
		SeekNextWhiteSpace(reader);
		auto data = reader.ToArray();
		data.Data()[data.Length() - 1] = '\0';
		return (const char *)data.Data();
	}

	unsigned CountOfFaceVertex(FileReader reader)
	{
		SeekNextWhiteSpace(reader);
		auto data = reader.ToArray();
		unsigned index = 1;

		// space ÇÃêîÇ™ñ Çç\ê¨Ç∑ÇÈÇΩÇﬂÇÃí∏ì_êîÇîªífÇ∑ÇÈ
		for (auto i : data)
			if (i == ' ')index++;

		return index;
	}

	void BuildFace( Collection<obj_face>& target, 
					unsigned face_count,
					const Collection<unsigned>& postion_indeces , 
					const Collection<unsigned>& normal_indeces ,
					const Collection<unsigned>& texcoord_indeces)
	{
		for (unsigned i = 0; i < face_count - 2; ++i)
		{
			obj_face f;

			if (postion_indeces.IsEmpty() == false)
			{
				f.idx_position[0] = postion_indeces[i * 3 + 0];
				f.idx_position[1] = postion_indeces[i * 3 + 1];
				f.idx_position[2] = postion_indeces[i * 3 + 2];
			}

			if (texcoord_indeces.IsEmpty() == false)
			{
				f.idx_texcoord[0] = texcoord_indeces[i * 3 + 0];
				f.idx_texcoord[1] = texcoord_indeces[i * 3 + 1];
				f.idx_texcoord[2] = texcoord_indeces[i * 3 + 2];
			}

			if (normal_indeces.IsEmpty() == false)
			{
				f.idx_normal[0] = normal_indeces[i * 3 + 0];
				f.idx_normal[1] = normal_indeces[i * 3 + 1];
				f.idx_normal[2] = normal_indeces[i * 3 + 2];
			}
			target.Add(f);
		}
	}

	Collection<unsigned> ConvertGeometoryToTriangleList(const unsigned* indexList,const unsigned indexCount)
	{
		Collection<unsigned> converted_indeces;
		converted_indeces.Reserve(indexCount * 3);
		const unsigned* h = &indexList[0];
		const unsigned* m = &indexList[0] + 1;
		for (unsigned i = 0; i < indexCount - 2; ++i)
		{
			converted_indeces.Add(*h);
			converted_indeces.Add(*(m++));
			converted_indeces.Add(*m);
		}
		return converted_indeces;
	}

	void WriteFloat1()
	{

	}

	bool TS::ObjParser::Parse(const char * filepath)
	{
		ManagedArray<unsigned char> binary = ReadBinary(filepath);

		if (binary == nullptr)
		{
			// file read error
			return false;
		}

		FileReader reader(binary);
		FilePathAnalyzer analizer(filepath);

		StringA use_material_name = "";
		obj_mesh default_mesh;
		obj_mesh& current_mesh = default_mesh;
		_file = obj_file();

		Collection<unsigned> null_collection;

		while (reader.Eof() == false)
		{
			auto&& line = reader.ReadLine();

			if (FirstOf(line.ToArray(), "v ")) 
				_file.positions.Add(ReadVector3(line));
			else if (FirstOf(line.ToArray(), "vn "))
				_file.normals.Add(ReadVector3(line));
			else if (FirstOf(line.ToArray(), "vt "))
				_file.texcoords.Add(ReadVector2(line));
			else if (FirstOf(line.ToArray(), "mtllib "))
				ReadMaterial(Path::Combine(analizer.GetDirectory(), ReadString(reader)));
			else if (FirstOf(line.ToArray(), "usemtl "))
				use_material_name = ReadString(reader);
			else if (FirstOf(line.ToArray(), "g "))
			{
				current_mesh.name = ReadString(reader);
				current_mesh.material_name = use_material_name;
				_file.meshes.Add(current_mesh);
				default_mesh = obj_mesh();
				current_mesh = default_mesh;
			}
			else if (FirstOf(line.ToArray(), "f "))
			{
				unsigned face_count = CountOfFaceVertex(reader);

				const unsigned max_face = 128;
				if (face_count < 3 || max_face < face_count )throw;

				unsigned pos_indeces[max_face];
				unsigned normal_indeces[max_face];
				unsigned texcoord_indeces[max_face];

				bool using_pos       = _file.positions.IsEmpty() == false;
				bool using_normal    = _file.positions.IsEmpty() == false;
				bool using_texcoords = _file.positions.IsEmpty() == false;

				if (using_pos & using_normal & using_texcoords)
				{
					for (unsigned i = 0; i < face_count; ++i)
					{
						const char * data = (const char *)(reader.ToArray().Data());
						if(i!=0)SeekNextWhiteSpace(reader);
						sscanf_s(data,"%d/%d/%d",
							&pos_indeces[i],
							&normal_indeces[i],
							&texcoord_indeces[i]);
					}

					BuildFace(current_mesh.face, face_count, 
						ConvertGeometoryToTriangleList(pos_indeces, face_count),
						ConvertGeometoryToTriangleList(normal_indeces, face_count),
						ConvertGeometoryToTriangleList(texcoord_indeces, face_count));
				}
				else if (using_pos & using_normal)
				{
					for (unsigned i = 0; i < face_count; ++i)
					{
						const char * data = (const char *)(reader.ToArray().Data());
						if (i != 0)SeekNextWhiteSpace(reader);
						sscanf_s(data, "%d/%d",
							&pos_indeces[i],
							&normal_indeces[i]);
					}

					BuildFace(current_mesh.face, face_count,
						ConvertGeometoryToTriangleList(pos_indeces, face_count),
						ConvertGeometoryToTriangleList(normal_indeces, face_count),
						null_collection);
				}
				else if (using_pos & using_texcoords)
				{
					for (unsigned i = 0; i < face_count; ++i)
					{
						const char * data = (const char *)(reader.ToArray().Data());
						if (i != 0)SeekNextWhiteSpace(reader);
						sscanf_s(data, "%d/%d",
							&pos_indeces[i],
							&texcoord_indeces[i]);
					}

					BuildFace(current_mesh.face, face_count,
						ConvertGeometoryToTriangleList(pos_indeces, face_count),
						null_collection,
						ConvertGeometoryToTriangleList(texcoord_indeces, face_count));
				}
				else if (using_pos)
				{
					for (unsigned i = 0; i < face_count; ++i)
					{
						const char * data = (const char *)(reader.ToArray().Data());
						if (i != 0)SeekNextWhiteSpace(reader);
						sscanf_s(data, "%d",
							&pos_indeces[i]);
					}

					BuildFace(current_mesh.face, face_count,
						ConvertGeometoryToTriangleList(pos_indeces, face_count),
						null_collection,
						null_collection);
				}
				else
					throw;
			}
		}

		if (_file.meshes.IsEmpty)
			_file.meshes.Add(current_mesh);

		return false;
	}

	bool TS::ObjParser::SaveAs(const char * filepath)
	{
		return false;
	}
	bool ObjParser::ReadMaterial(const char * material_file_path)
	{
		ManagedArray<unsigned char> binary = ReadBinary(material_file_path);

		if (binary == nullptr)
		{
			// file read error
			return false;
		}

		FileReader reader(binary);

		obj_material material;
		material.file_path = material_file_path;

		obj_material& current_material = material;

		while (reader.Eof() == false)
		{
			auto&& line = reader.ReadLine();
			if (FirstOf(line.ToArray(), "newmtl "))
			{
				material.name = ReadString(line);
				_file.materials.Add(material);
				current_material = _file.materials[_file.materials.Length() - 1];
			}
			else if (FirstOf(line.ToArray(), "Kd "))
				current_material.diffuse = ReadVector3(line);
			else if (FirstOf(line.ToArray(), "Ka "))
				current_material.ambient = ReadVector3(line);
			else if (FirstOf(line.ToArray(), "Ks "))
				current_material.specluer = ReadVector3(line);
			else if (FirstOf(line.ToArray(), "Ns "))
				current_material.specluerPower = ReadFloat(line);
			else if (FirstOf(line.ToArray(), "d "))
				current_material.alpha = ReadFloat(line);
			else if (FirstOf(line.ToArray(), "Tr "))
				current_material.alpha = 1 - ReadFloat(line);
			else if (FirstOf(line.ToArray(), "Tf "))
				current_material.tf = ReadVector3(line);
			else if (FirstOf(line.ToArray(), "ni "))
				current_material.ni = ReadFloat(line);
			else if (FirstOf(line.ToArray(), "illum "))
				current_material.luminous = ReadFloat(line);
			else if (FirstOf(line.ToArray(), "map_Kd "))
				current_material.diffuse_map = ReadString(line);
			else if (FirstOf(line.ToArray(), "map_Ka "))
				current_material.ambient_map = ReadString(line);
			else if (FirstOf(line.ToArray(), "map_Ks "))
				current_material.specluer_map = ReadString(line);
			else if (FirstOf(line.ToArray(), "map_Ns "))
				current_material.hightlight_map = ReadString(line);
			else if (FirstOf(line.ToArray(), "Bump "))
				current_material.bump_map = ReadString(line);
			else if (FirstOf(line.ToArray(), "map_Bump "))
				current_material.bump_map = ReadString(line);
			else if (FirstOf(line.ToArray(), "map_d "))
				current_material.alpha_map = ReadString(line);
		}

		return true;
	}
}

