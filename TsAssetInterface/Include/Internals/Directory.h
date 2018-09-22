#pragma once

namespace TS
{
	class Directory
	{
	public:
		//! Constructor
		Directory(const StringA& directory, bool enableSubgolder = true);

		Directory& RegisterFilter(const Collection<StringA>& filters);

		Collection<StringA> GetFiles();
	protected:
		StringA _directory;
		Collection<StringA> _filters;
		bool _seachInSubDirectory;
	};
}