#pragma once

namespace TS 
{
	struct obj_face
	{
		using int3 = int[3];
		int3 idx_position{ -1,-1,-1 };
		int3 idx_normal  { -1,-1,-1 };
		int3 idx_texcoord{ -1,-1,-1 };
	};

	struct obj_material
	{
		StringA name;
		StringA file_path;

		Vector3 diffuse{ -1 ,-1,-1 };
		Vector3 ambient{ -1 ,-1,-1 };
		Vector3 specluer{ -1 ,-1,-1 };
		float   specluerPower{ -1 };
		float   alpha{ -1 };
		float   luminous{ -1 };			 // 輝度
		float   sharpness{ -1 };		 // 0 - 1000 default 60
		float   ni{ -1 };				 //屈折率    0.001 - 10
		float   ns{ -1 };				 //鏡面指数  0 - 1000
		Vector3 tf{ -1 , -1 , -1};		 //スペクトル係数

		StringA diffuse_map{ "" };
		StringA bump_map{ "" };
		StringA ambient_map{ "" };
		StringA specluer_map{ "" };
		StringA hightlight_map{ "" };
		StringA alpha_map{ "" };
	};

	struct obj_mesh
	{
		Collection<obj_face> face;
		StringA material_name;
		StringA name;
	};

	struct obj_file
	{
		Collection<Vector3> positions;
		Collection<Vector3> normals;
		Collection<Vector2> texcoords;
		Collection<obj_mesh> meshes;
		Collection<obj_material> materials;
	};

	class ObjParser : public I3ModelParser
	{
	public:
		bool Parse (const char * filepath)override;
		bool SaveAs(const char * filepath)override;
	private:
		bool ReadMaterial(const char * material_name);
		bool SaveMaterial(const char * material_name);
		obj_file _file;
	};
}

