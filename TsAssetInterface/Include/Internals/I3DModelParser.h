#pragma once

namespace TS 
{
	class I3ModelParser
	{
	public:
		virtual bool Parse(Binary& binary) = 0;
		virtual bool Write(const char * output);
	};
}
