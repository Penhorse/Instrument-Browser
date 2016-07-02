/*
  ==============================================================================

    ISMSnoopWrapper.cpp
    Created: 31 Oct 2015 6:52:56pm
    Author:  Chris

  ==============================================================================
*/

#include "ISMSnoopWrapper.h"

#include <deque>

ISMSnoopWrapper::ISMSnoopWrapper() :
	library_(library_filename())
{
	// nothing
}

bool ISMSnoopWrapper::load()
{
#if JUCE_MAC
    //assumes libismsnoop.dylib has been placed inside InstrumentBrowser.app/Contents/MacOS
    
    const std::string libSearchPath =
        File::getSpecialLocation(File::invokedExecutableFile).getParentDirectory().getFullPathName().toStdString();
    const std::deque<std::string> libSearchPaths {libSearchPath};
    
	if (!library_.load(libSearchPaths))
	{
		return false;
	}
    
#else
    
    if (!library_.load())
    {
        return false;
    }
    
#endif
	
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

