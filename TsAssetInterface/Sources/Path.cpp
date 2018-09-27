#include "pch.h"
#include "Path.h"

namespace TS 
{
	const char Path::Separators[] = {'\\','/'};
	StringA Path::Combine(const StringA & parent, const StringA & file)
	{
		return parent + "/" + file;
	}
	StringA Path::GetFileName(const StringA & filepath)
	{
		auto index = filepath.Rfind('/');

		if (index == -1)
			index = filepath.Rfind('\\');

		if (index == -1)
			return filepath;

		return filepath.SubString(index);
	}
	StringA Path::GetFileNameWithoutExtension(const StringA & filepath)
	{
		StringA result = GetFileName(filepath);

	    const auto index = result.Find('.');
		if (index == -1)
			return "";
		return result.SubString(0,index);
	}
	StringA Path::GetFullPath(const StringA & filepath)
	{
		char szFullPath[MAX_PATH] = { '\0' };
		char *szFilePart;

		GetFullPathNameA(
			filepath,
			sizeof(szFullPath) / sizeof(szFullPath[0]),
			szFullPath,
			&szFilePart);

		return szFullPath;
	}
	StringA Path::GetParent(const StringA & filepath)
	{
		auto index = filepath.Rfind('/');

		if (index == -1)
			index = filepath.Rfind('\\');

		if (index == -1)
			return filepath;

		return filepath.SubString(0, index);
	}
	HashCode Path::GetHashCode(const StringA & str)
	{
		HashCode h = 0;
		const char* val = str;
		int len = str.Length();

		for (int i = 0; i < len; i++) {
			h = 31 * h + val[i];
		}
		return h;
	}
	StringA Path::GetExtension(const StringA & filepath)
	{
		StringA result = GetFileName(filepath);

		const auto index = result.Find('.');
		if (index == -1)
			return "";
		return result.SubString(index);
	}
	StringA Path::ToUpper(const StringA & filepath)
	{
		StringA result = filepath;
		for (auto& c : result)
			c = toupper(c);
		return result;
	}
	StringA Path::ToLower(const StringA & filepath)
	{
		StringA result = filepath;
		for (auto& c : result)
			c = tolower(c);
		return result;
	}
}