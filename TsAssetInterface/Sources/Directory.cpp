#include "pch.h"
#include "Path.h"
#include "Directory.h"

namespace TS 
{
	TS::Directory::Directory(const StringA & directory, bool enableSubgolder)
		:_filters(),_directory(directory)
	{
	}
	Directory & Directory::RegisterFilter(const Collection<StringA>& ignores)
	{
		_filters = ignores;
		return *this;
	}
	Collection<StringA> Directory::GetFiles()
	{
		Collection<StringA> fileList;
		WIN32_FIND_DATAA tFindFileData;
		// 最初に一致するファイルを取得
		HANDLE hFile = ::FindFirstFileA(_directory + "*", &tFindFileData);

		if (INVALID_HANDLE_VALUE == hFile) {
			return fileList;
		}
		do {

			StringA filename = tFindFileData.cFileName;

			//! 自身を参照すると無限ループになるので回避
			if (filename == "." || filename == "..")
				continue;

			// フォルダかどうかを判定
			if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//サブフォルダの検索
				if (_seachInSubDirectory == false)
					continue;

				Directory subdirectory( _directory + filename,true);
				subdirectory.RegisterFilter(_filters);
				Collection<StringA>&& subFolderFiles = subdirectory.GetFiles();
				fileList.AddRange(subdirectory.GetFiles());
			}
			else
			{
				// 拡張子が設定されていない場合は全てのファイルを格納
				if (_filters.IsEmpty())
					fileList.Add(_directory + filename);
				else
				{
					//拡張子が設定されている場合は指定の拡張子のファイルならリストに追加
					for (auto&& e : _filters)
						if (filename.Contain(e))
						{
							fileList.Add(_directory + filename);
							break;
						}
				}
			}

			// 次に一致するファイルの検索
		} while (::FindNextFileA(hFile, &tFindFileData));

		// 検索ハンドルを閉じる
		::FindClose(hFile);
		return fileList;
	}
}

