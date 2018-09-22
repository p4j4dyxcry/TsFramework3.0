#pragma once

namespace TS 
{
	class Path
	{
	public:		
		static StringA Combine(const StringA& parent, const StringA& file);

		static StringA GetFileName(const StringA& filepath);

		static StringA GetFileNameWithoutExtension(const StringA& filepath);

		static StringA GetFullPath(const StringA& filepath);

		static StringA GetParent(const StringA& filepath);

		static HashCode GetHashCode(const StringA& str);

		static StringA GetExtension(const StringA& filepath);

		static StringA ToUpper(const StringA& filepath);

		static StringA ToLower(const StringA& filepath);
	private:
		static const char Separators[];
	};
}

