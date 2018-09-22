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
		// �ŏ��Ɉ�v����t�@�C�����擾
		HANDLE hFile = ::FindFirstFileA(_directory + "*", &tFindFileData);

		if (INVALID_HANDLE_VALUE == hFile) {
			return fileList;
		}
		do {

			StringA filename = tFindFileData.cFileName;

			//! ���g���Q�Ƃ���Ɩ������[�v�ɂȂ�̂ŉ��
			if (filename == "." || filename == "..")
				continue;

			// �t�H���_���ǂ����𔻒�
			if (tFindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//�T�u�t�H���_�̌���
				if (_seachInSubDirectory == false)
					continue;

				Directory subdirectory( _directory + filename,true);
				subdirectory.RegisterFilter(_filters);
				Collection<StringA>&& subFolderFiles = subdirectory.GetFiles();
				fileList.AddRange(subdirectory.GetFiles());
			}
			else
			{
				// �g���q���ݒ肳��Ă��Ȃ��ꍇ�͑S�Ẵt�@�C�����i�[
				if (_filters.IsEmpty())
					fileList.Add(_directory + filename);
				else
				{
					//�g���q���ݒ肳��Ă���ꍇ�͎w��̊g���q�̃t�@�C���Ȃ烊�X�g�ɒǉ�
					for (auto&& e : _filters)
						if (filename.Contain(e))
						{
							fileList.Add(_directory + filename);
							break;
						}
				}
			}

			// ���Ɉ�v����t�@�C���̌���
		} while (::FindNextFileA(hFile, &tFindFileData));

		// �����n���h�������
		::FindClose(hFile);
		return fileList;
	}
}

