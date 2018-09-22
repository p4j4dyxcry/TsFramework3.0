#include "pch.h"
#include "Path.h"
#include "FilePathAnalizer.h"

namespace TS 
{
	inline TS::FilePathAnalyzer::FilePathAnalyzer(const StringA & filePath)
	{
		analize(filePath);
	}

	inline const StringA & TS::FilePathAnalyzer::GetFileName() const
	{
		return _fileName;
	}

	inline const StringA & TS::FilePathAnalyzer::GetFullPath() const
	{
		return _fullPath;
	}

	inline const StringA & TS::FilePathAnalyzer::GetExtension() const
	{
		return _extension;
	}

	inline const StringA & TS::FilePathAnalyzer::GetDirectory() const
	{
		return _fullDirectory;
	}

	inline FilePathAnalyzer & TS::FilePathAnalyzer::Rename(const StringA & newName)
	{
		analize(Path::Combine(_fullDirectory, newName));
		return *this;
	}

	inline FilePathAnalyzer & TS::FilePathAnalyzer::ReExtencion(const StringA & extension)
	{
		return Rename(Path::GetFileNameWithoutExtension(_fileName) + extension);
	}

	inline void TS::FilePathAnalyzer::analize(const StringA & filePath)
	{
		_filePath = filePath;
		_extension = Path::GetExtension(filePath);
		_fullPath = Path::GetFullPath(filePath);
		_fileName = Path::GetFileName(filePath);
		_fullDirectory = Path::GetParent(_fullPath);
	}
}


