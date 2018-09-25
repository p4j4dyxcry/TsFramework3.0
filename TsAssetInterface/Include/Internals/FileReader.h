#pragma once

namespace TS 
{
	class FileReader : public BinaryReader
	{
	public:
		FileReader(const Binary& binary):BinaryReader(binary){}

	    FileReader ReadLine();

	    Array<unsigned char> ToArray() const;

	    unsigned Current() const;
	};
}

