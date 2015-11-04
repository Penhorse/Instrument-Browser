#include "Instrument.h"

#include <cctype>
#include <rtw/algorithms.hpp>
#include <rtw/filesystem.hpp>
#include <rtw/strings.hpp>

bool Instrument::Icon::operator<(const Icon & rhs) const
{
	if(width < rhs.width) return true;
	if(width > rhs.width) return false;

	return height < rhs.height;
}

bool Instrument::operator<(const Instrument & rhs) const
{
	if(path < rhs.path) return true;
	if(path > rhs.path) return false;

	if(name < rhs.name) return true;
	if(name > rhs.name) return false;

	return icon < rhs.icon;
}

bool Instrument::matches_filter(const std::string & filter) const
{
	const auto tokens = rtw::strings::split(filter, ::isspace);

	for(const auto & token : tokens)
	{
		const auto token_upper = rtw::strings::toupper(token);

		for(const auto & name : get_subdirectory_names(3))
		{
			const auto name_upper = rtw::strings::toupper(name);

			if(name_upper.find(token_upper) != std::string::npos)
			{
				return true;
			}
		}

		const auto name_upper = rtw::strings::toupper(name);

		if(name_upper.find(token_upper) == std::string::npos)
		{
			return false;
		}
	}

	return true;
}

std::vector<std::string> Instrument::get_subdirectory_names(int levels) const
{
	auto parts = rtw::strings::split(path, rtw::fs::is_path_separator);

	if(!parts.empty())
	{
		parts.pop_back();
	}

	return rtw::algo::get_last_n_elements(parts, levels);
}
