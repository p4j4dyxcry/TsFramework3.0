#pragma once

namespace TS
{	
	class FilePathAnalyzer
	{
	public:
		FilePathAnalyzer() {};

		FilePathAnalyzer(const StringA& filePath);;

		const StringA& GetFileName()const;

		const StringA& GetFullPath()const;

		const StringA& GetExtension()const;

		const StringA& GetDirectory()const;

		FilePathAnalyzer& Rename(const StringA& newName);

		FilePathAnalyzer& ReExtencion(const StringA& extension);
	protected:

		void analize(const StringA& filePath);

		StringA _filePath;		 //���[�J���̃t�@�C���p�X
		StringA _fullPath;		 //�t���p�X
		StringA _extension;		 //�t�@�C���̊g���q
		StringA _fileName;		 //�f�B���N�g�����܂܂Ȃ��t�@�C���̖��O
		StringA _fullDirectory;  //�t�@�C�����܂܂Ȃ��t���p�X�̃f�B���N�g���\��
	};
}