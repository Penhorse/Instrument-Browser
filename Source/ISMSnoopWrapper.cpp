/*
  ==============================================================================

    ISMSnoopWrapper.cpp
    Created: 31 Oct 2015 6:52:56pm
    Author:  Chris

  ==============================================================================
*/

#include "ISMSnoopWrapper.h"

ISMSnoopWrapper::ISMSnoopWrapper() :
	library_(library_filename())
{
	// nothing
}

bool ISMSnoopWrapper::load()
{
	if (!library_.load())
	{
		return false;
	}

	return true;
}

std::string ISMSnoopWrapper::library_filename()
{
	return rtw::dylib::get_filename("ismsnoop");
}
