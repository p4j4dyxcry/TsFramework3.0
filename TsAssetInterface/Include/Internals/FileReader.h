#pragma once

namespace TS 
{
	class FileReader : public BinaryReader
	{
	public:
		FileReader(const Binary& binary):BinaryReader(binary){}

		FileReader ReadLine()
		{
			unsigned start = _current;
			SeekElement('\n');
			return FileReader(Binary(&_binary[start], _current - start));
		}

	};
}

