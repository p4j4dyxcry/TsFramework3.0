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

		StringA _filePath;		 //ローカルのファイルパス
		StringA _fullPath;		 //フルパス
		StringA _extension;		 //ファイルの拡張子
		StringA _fileName;		 //ディレクトリを含まないファイルの名前
		StringA _fullDirectory;  //ファイルを含まないフルパスのディレクトリ構成
	};
}