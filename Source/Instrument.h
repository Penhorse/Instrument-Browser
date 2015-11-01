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

struct Instrument
{
	struct Icon
	{
		int width;
		int height;
		std::vector<char> bytes;
	};

	std::string path;
	Icon icon;
};

#endif  // INSTRUMENT_H_INCLUDED
