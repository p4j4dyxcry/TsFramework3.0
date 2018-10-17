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

        bool Contain(const char* tag) const;

        int Find(const char * tag)const;
	};
}

