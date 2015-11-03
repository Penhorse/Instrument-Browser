/*
  ==============================================================================

    InstrumentLoader.h
    Created: 31 Oct 2015 2:36:54pm
    Author:  Chris

  ==============================================================================
*/

#ifndef INSTRUMENTLOADER_H_INCLUDED
#define INSTRUMENTLOADER_H_INCLUDED

#include <memory>

#include "../JuceLibraryCode/JuceHeader.h"

#include "Instrument.h"
#include "ISMSnoopWrapper.h"

class ErrorReceiver;
class InstrumentReceiver;

class InstrumentLoader : public Thread
{

public:

	InstrumentLoader(
			const StringArray & directories,
			std::shared_ptr<ISMSnoopWrapper> ismsnoop,
			InstrumentReceiver * instrument_receiver,
			ErrorReceiver * error_receiver);

	void run();

private:

	int try_load_directory(const File & dir) const;
	int load_directory(const File & dir) const;
	void load_file(const File & file) const;
	std::string get_name(ISMSnoopInstrument * ism) const;
	Instrument::Icon get_icon(ISMSnoopInstrument * ism) const;

	StringArray directories_;
	std::shared_ptr<ISMSnoopWrapper> ismsnoop_;
	InstrumentReceiver * instrument_receiver_;
	ErrorReceiver * error_receiver_;
};


#endif  // INSTRUMENTLOADER_H_INCLUDED
