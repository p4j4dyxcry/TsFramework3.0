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
		while (tag[index] != '\0')
		{
			if (binary[index] != (unsigned char)(tag[index]))
				return false;
			index++;
		}
		return true;
	}

	void SeekNextWhiteSpace(FileReader& reader)
	{
		auto data = reader.ToArray();
		unsigned index = 0;
		while(index < data.Length())
		{
			if (data[index++] == ' ')
			{
				while (data[index] == ' ') 
					index++;
				reader.Seek(reader.Current() + index);
				break;
			}
		}
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
			"%f %f %f",
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

	unsigned CountOfFaceVertex(FileReader& reader)
	{
		reader.Seek(0);
		SeekNextWhiteSpace(reader);
		auto data = reader.ToArray();
		reader.Seek(0);

		unsigned index = 1;

		// space ‚Ì”‚ª–Ê‚ð\¬‚·‚é‚½‚ß‚Ì’¸“_”‚ð”»’f‚·‚é
		for (unsigned i = 0 ; i < data.Length() - 1 ; ++i)
		{
			if (data[i + 1] == '\n' || data[i + 1] == '\r' || data[i + 1] == '\r\n')
				break;

			if (data[i] == ' ' && data[i + 1] != ' ')
				index++;
		}
		
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

	bool HasValue(float value)
	{
		return value >= 0;
	}

	bool HasValue(const Vector3& value)
	{
		return value.x >= 0;
	}

	bool HasValue(const StringA& value)
	{
		return value.IsNullOrEmpty() == false;
	}

	void WriteFloat(std::ofstream& ofs, const char* element, float param,bool no_check = false)
	{
		if(no_check | HasValue(param))
			ofs << element << " " << param << std::endl;
	}

	void WriteVector3(std::ofstream& ofs, const char* element, const Vector3& param, bool no_check = false)
	{
		if (no_check | HasValue(param))
			ofs << element << " " << param.x <<
							  " " << param.y << 
							  " " << param.z << std::endl;
	}

	void WriteString(std::ofstream& ofs, const char* element, const StringA& param)
	{
		if (HasValue(param))
			ofs << element << " " << param.Data() << std::endl;
	}

	bool TS::ObjParser::Parse(const char * filepath)
	{
		FilePathAnalyzer analizer1(filepath);
		ManagedArray<unsigned char> binary = ReadBinary(filepath);

		if (binary == nullptr)
		{
			// file read error
			return false;
		}

		FileReader stream(binary);
		FilePathAnalyzer analizer(filepath);

		StringA use_material_name = "";
		Collection<obj_mesh> meshes;
		meshes.Reserve(16);
		meshes.Add(obj_mesh());

		obj_mesh& current_mesh = meshes[0];
		_file = obj_file();

		Collection<unsigned> null_collection;

		while (stream.Eof() == false)
		{
			auto&& line = stream.ReadLine();

			if (FirstOf(line.ToArray(), "v ")) 
				_file.positions.Add(ReadVector3(line));
			else if (FirstOf(line.ToArray(), "vn "))
				_file.normals.Add(ReadVector3(line));
			else if (FirstOf(line.ToArray(), "vt "))
				_file.texcoords.Add(ReadVector2(line));
			else if (FirstOf(line.ToArray(), "mtllib "))
				ReadMaterial(Path::Combine(analizer.GetDirectory(), ReadString(line)));
			else if (FirstOf(line.ToArray(), "usemtl "))
				use_material_name = ReadString(line);
			else if (FirstOf(line.ToArray(), "g "))
			{
				meshes.Add(obj_mesh());
				current_mesh = meshes[meshes.Length() - 1];
				current_mesh.name = ReadString(line);
				current_mesh.material_name = use_material_name;

			}
			else if (FirstOf(line.ToArray(), "f "))
			{
				unsigned face_count = CountOfFaceVertex(line);

				const unsigned max_face = 128;
				if (face_count < 3 || max_face < face_count )throw;

				unsigned pos_indeces[max_face];
				unsigned normal_indeces[max_face];
				unsigned texcoord_indeces[max_face];

				bool using_pos       = _file.positions.IsEmpty() == false;
				bool using_normal    = _file.normals.IsEmpty() == false;
				bool using_texcoords = _file.texcoords.IsEmpty() == false;

				if (using_pos & using_normal & using_texcoords)
				{
					for (unsigned i = 0; i < face_count; ++i)
					{
						SeekNextWhiteSpace(line);
						const char * data = (const char *)(line.ToArray().Data());
						if (sscanf_s(data, "%d/%d/%d", &pos_indeces[i], &normal_indeces[i], &texcoord_indeces[i]) != 3)
							sscanf_s(data, "%d//%d//%d", &pos_indeces[i], &normal_indeces[i], &texcoord_indeces[i]);
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
						SeekNextWhiteSpace(line);
						const char * data = (const char *)(line.ToArray().Data());
						if (sscanf_s(data, "%d/%d", &pos_indeces[i], &normal_indeces[i]) != 2)
							sscanf_s(data, "%d//%d", &pos_indeces[i], &normal_indeces[i]);
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
						SeekNextWhiteSpace(line);
						const char * data = (const char *)(line.ToArray().Data());
						if (sscanf_s(data, "%d/%d", &pos_indeces[i], &texcoord_indeces[i]) == 0)
							sscanf_s(data, "%d//%d", &pos_indeces[i], &texcoord_indeces[i]);
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
						SeekNextWhiteSpace(line);
						const char * data = (const char *)(line.ToArray().Data());
						sscanf_s(data, "%d",&pos_indeces[i]);
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

		_file.meshes.AddRange(meshes);

		return true;
	}

	bool TS::ObjParser::SaveAs(const char * filepath)
	{
		std::ofstream ofs(filepath);

		FilePathAnalyzer analizer(filepath);

		FilePathAnalyzer materialAnalizer(filepath);
		materialAnalizer.ReExtencion(".mtl");
		if (_file.materials.Length() > 0) 
		{
			WriteString(ofs, "mtlib", materialAnalizer.GetFileName());
			SaveMaterial(materialAnalizer.GetFullPath());
		}

		for (auto v  : _file.positions) WriteVector3(ofs, "v", v);
		for (auto vn : _file.normals)   WriteVector3(ofs, "vn", vn); 
		for (auto vt : _file.texcoords) WriteVector3(ofs, "vt", vt); 

		bool has_pos    = _file.positions.Length() > 0;
		bool has_normal = _file.normals.Length() > 0;
		bool has_uv     = _file.texcoords.Length() > 0;

		for (auto mesh : _file.meshes)
		{
			WriteString(ofs, "g", mesh.name);
			WriteString(ofs, "usemtl", mesh.material_name);

			for (auto& face : mesh.face)
			{
				ofs << "f ";
				if (has_pos & has_normal & has_uv)
				{
					ofs << face.idx_position[0] << "/"
						<< face.idx_texcoord[0] << "/"
						<< face.idx_normal[0]   << "/" << " "

						<< face.idx_position[1] << "/"
						<< face.idx_texcoord[1] << "/"
						<< face.idx_normal[1]   << "/" << " "

						<< face.idx_position[2] << "/"
						<< face.idx_texcoord[2] << "/"
						<< face.idx_normal[2]   << std::endl;
				}

				else if (has_pos & has_normal )
				{
					ofs << face.idx_position[0] << "/"
						<< face.idx_normal[0]  << "/" << " "

						<< face.idx_position[1] << "/"
						<< face.idx_normal[1]   << "/" << " "

						<< face.idx_position[2] << "/"
						<< face.idx_normal[2]   << std::endl;
				}

				else if (has_pos & has_uv)
				{
					ofs << face.idx_position[0] << "/"
						<< face.idx_texcoord[0] << "/" << " "

						<< face.idx_position[1] << "/"
						<< face.idx_texcoord[1] << "/" << " "

						<< face.idx_position[2] << "/"
						<< face.idx_texcoord[2] << std::endl;
				}

				else if (has_pos)
				{
					ofs << face.idx_position[0] << "/" << " "
						<< face.idx_position[1] << "/" << " "
						<< face.idx_position[2] << "/" << std::endl;
				}
			}
		}

		return true;
	}
	bool ObjParser::ReadMaterial(const char * material_file_path)
	{
		ManagedArray<unsigned char> binary = ReadBinary(material_file_path);

		if (binary == nullptr)
		{
			// file read error
			return false;
		}

		FileReader stream(binary);

		_file.materials.Add(obj_material());
		obj_material& current_material = _file.materials[0];
		current_material.file_path = material_file_path;


		while (stream.Eof() == false)
		{
			auto&& line = stream.ReadLine();
			if (FirstOf(line.ToArray(), "newmtl "))
			{
				_file.materials.Add(obj_material());
				current_material = _file.materials[_file.materials.Length() - 1];

				current_material.name = ReadString(line);
				current_material.file_path = material_file_path;
				
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

	bool ObjParser::SaveMaterial(const char * material_file_path)
	{
		std::ofstream ofs(material_file_path);

		for(const auto& material : _file.materials)
		{
			WriteString(ofs, "newmtl", material.name);
			WriteVector3(ofs, "Kd", material.diffuse);
			WriteVector3(ofs, "Ka", material.ambient);
			WriteVector3(ofs, "Ks", material.specluer);
			WriteFloat(ofs, "Ns", material.specluerPower);
			WriteFloat(ofs, "d", material.alpha);
			WriteFloat(ofs, "illum", material.luminous);
			WriteVector3(ofs, "Tf", material.tf);
			WriteFloat(ofs, "Ni", material.ni);
			WriteString(ofs, "map_Kd", material.diffuse_map);
			WriteString(ofs, "map_Ka", material.ambient_map);
			WriteString(ofs, "map_Ks", material.specluer_map);
			WriteString(ofs, "map_Ns", material.hightlight_map);
			WriteString(ofs, "map_Bump", material.bump_map);
			WriteString(ofs, "map_d", material.alpha_map);
		}
		return true;
	}
}

