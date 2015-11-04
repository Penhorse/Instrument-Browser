/*
  ==============================================================================

    Instrument.h
    Created: 31 Oct 2015 6:05:10pm
    Author:  Chris

  ==============================================================================
*/

#ifndef INSTRUMENT_H_INCLUDED
#define INSTRUMENT_H_INCLUDED

#include <string>
#include <vector>

class Instrument
{

public:

	struct Icon
	{
		int width;
		int height;
		std::vector<char> bytes;

		bool operator<(const Icon & rhs) const;
	};

	std::string path;
	std::string name;
	Icon icon;

	bool operator<(const Instrument & rhs) const;

	bool matches_filter(const std::string & filter) const;

private:

	std::vector<std::string> get_subdirectory_names(int levels) const;

};

#endif  // INSTRUMENT_H_INCLUDED
