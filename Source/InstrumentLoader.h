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

class InstrumentReceiver;
class ISMSnoopWrapper;

class InstrumentLoader : public Thread
{

public:

	InstrumentLoader(const StringArray & directories, std::shared_ptr<ISMSnoopWrapper> ismsnoop, InstrumentReceiver * receiver);

	void run();

private:

	StringArray directories_;
	InstrumentReceiver * receiver_;
};


#endif  // INSTRUMENTLOADER_H_INCLUDED
