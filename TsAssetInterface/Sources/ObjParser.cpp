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
	bool ReadVector2(const StringA& source, const char* element, Vector2& result)
	{
		auto p = source.Find(element);
		size_t elementSize = StringA::StringLength(element);
		if (p != -1)
		{
			sscanf_s(source.SubString(p + elementSize),
				"%f %f",
				&result.x,
				&result.y);
			return true;
		}
		return false;
	};

	bool ReadVector3(const StringA& source, const char* element, Vector3& result)
	{
		auto p = source.Find(element);
		size_t elementSize = StringA::StringLength(element);
		if (p != -1)
		{
			sscanf_s(source.SubString(p + elementSize),
				"%f %f %f",
				&result.x,
				&result.y,
				&result.z);
			return true;
		}
		return false;
	};

	bool TS::ObjParser::Parse(const char * filepath)
	{


		FilePathAnalyzer analizer(filepath);

		BYTE flag = 0;
		enum
		{
			USE_POS = 0x01,
			USE_NORMAL = 0x02,
			USE_UV = 0x04,
		};

		return false;
	}

	bool TS::ObjParser::SaveAs(const char * filepath)
	{
		return false;
	}
}

