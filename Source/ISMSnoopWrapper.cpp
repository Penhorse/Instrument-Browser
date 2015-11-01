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
	
	if (!(open = library_.get_function<Sig_ismsnoop_open>("ismsnoop_open"))) return false;
	if (!(close = library_.get_function<Sig_ismsnoop_close>("ismsnoop_close"))) return false;
	if (!(get_panel_icon_size = library_.get_function<Sig_ismsnoop_get_panel_icon_size>("ismsnoop_get_panel_icon_size"))) return false;
	if (!(get_panel_icon_bytes = library_.get_function<Sig_ismsnoop_get_panel_icon_bytes>("ismsnoop_get_panel_icon_bytes"))) return false;
	if (!(get_name = library_.get_function<Sig_ismsnoop_get_name>("ismsnoop_get_name"))) return false;

	return true;
}

std::string ISMSnoopWrapper::library_filename()
{
	return rtw::dylib::get_filename("ismsnoop");
}

