#include "pch.h"
#include "Path.h"
#include "FilePathAnalizer.h"

namespace TS 
{
	TS::FilePathAnalyzer::FilePathAnalyzer(const StringA & filePath)
	{
		analize(filePath);
	}

	const StringA & TS::FilePathAnalyzer::GetFileName() const
	{
		return _fileName;
	}

	const StringA & TS::FilePathAnalyzer::GetFullPath() const
	{
		return _fullPath;
	}

	const StringA & TS::FilePathAnalyzer::GetExtension() const
	{
		return _extension;
	}

	const StringA & TS::FilePathAnalyzer::GetDirectory() const
	{
		return _fullDirectory;
	}

	FilePathAnalyzer & TS::FilePathAnalyzer::Rename(const StringA & newName)
	{
		analize(Path::Combine(_fullDirectory, newName));
		return *this;
	}

	FilePathAnalyzer & TS::FilePathAnalyzer::ReExtencion(const StringA & extension)
	{
		return Rename(Path::GetFileNameWithoutExtension(_fileName) + extension);
	}

	void TS::FilePathAnalyzer::analize(const StringA & filePath)
	{
		_filePath = filePath;
		_extension = Path::GetExtension(filePath);
		_fullPath = Path::GetFullPath(filePath);
		_fileName = Path::GetFileName(filePath);
		_fullDirectory = Path::GetParent(_fullPath);
	}
}


