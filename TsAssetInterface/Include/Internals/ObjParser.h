#pragma once

namespace TS 
{
	enum class obj_material_params : unsigned
	{
		Diffuse    = 0x0001,
		Ambient    = 0x0002,
		Specluer   = 0x0004,
		Power      = 0x0008,
		Alpha      = 0x0010,
		Lum        = 0x0020,
		Sharpness  = 0x0040,
		Ni         = 0x0080,
		Ns         = 0x0100,
		Tf         = 0x0200
	};

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

		Vector3 diffuse;
		Vector3 ambient;
		Vector3 specluer;
		float   specluerPower;
		float   alpha;
		float   luminous;  // 輝度
		float   sharpness; // 0 - 1000 default 60
		float   ni;		   //屈折率    0.001 - 10
		float   ns;		   //鏡面指数  0 - 1000
		Vector3 tf;		   //スペクトル係数
		obj_material_params using_flags;
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
	};
}

