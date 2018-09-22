#pragma once

namespace TS 
{
	class I3ModelParser
	{
	public:
		virtual bool Parse(const char * filepath) = 0;
		virtual bool SaveAs(const char * filepath) = 0;
	};
}
