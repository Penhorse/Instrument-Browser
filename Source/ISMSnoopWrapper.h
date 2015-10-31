/*
  ==============================================================================

    ISMSnoopWrapper.h
    Created: 31 Oct 2015 6:52:56pm
    Author:  Chris

  ==============================================================================
*/

#ifndef ISMSNOOPWRAPPER_H_INCLUDED
#define ISMSNOOPWRAPPER_H_INCLUDED

#include <rtw/dynamic_library.hpp>
#include <ismsnoop/ismsnoop.h>

#include "../JuceLibraryCode/JuceHeader.h"

class ISMSnoopWrapper
{

public:

	ISMSnoopWrapper();

	bool load();
	static std::string library_filename();

private:

	using Sig_ismsnoop_open = ISMSnoopInstrument*(const char*);
	using Sig_ismsnoop_close = void(ISMSnoopInstrument*);
	using Sig_ismsnoop_get_panel_icon_size = void(ISMSnoopInstrument*, int*, int*, int*);
	using Sig_ismsnoop_get_panel_icon_bytes = void(ISMSnoopInstrument*, char*);

	rtw::DynamicLibrary library_;

};

#endif  // ISMSNOOPWRAPPER_H_INCLUDED
