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

	using Sig_ismsnoop_open = ISMSnoopInstrument*(const char*);
	using Sig_ismsnoop_close = void(ISMSnoopInstrument*);
	using Sig_ismsnoop_get_panel_icon_size = void(ISMSnoopInstrument*, int*, int*, int*);
	using Sig_ismsnoop_get_panel_icon_bytes = void(ISMSnoopInstrument*, char*);

	std::function<Sig_ismsnoop_open> open;
	std::function<Sig_ismsnoop_close> close;
	std::function<Sig_ismsnoop_get_panel_icon_size> get_panel_icon_size;
	std::function<Sig_ismsnoop_get_panel_icon_bytes> get_panel_icon_bytes;

private:

	rtw::DynamicLibrary library_;

};

#endif  // ISMSNOOPWRAPPER_H_INCLUDED
