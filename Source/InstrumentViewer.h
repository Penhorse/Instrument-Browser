/*
  ==============================================================================

    InstrumentViewer.h
    Created: 31 Oct 2015 5:47:33pm
    Author:  Chris

  ==============================================================================
*/

#ifndef INSTRUMENTVIEWER_H_INCLUDED
#define INSTRUMENTVIEWER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include <mutex>

#include "InstrumentReceiver.h"
#include "InstrumentIcon.h"

//==============================================================================
class InstrumentViewer : public Component, public InstrumentReceiver, public AsyncUpdater
{
public:
    InstrumentViewer();
    ~InstrumentViewer();

    void paint (Graphics&);
    void resized();

	void receive_instrument(const Instrument & instrument) override;
	void refresh_instruments() override;

	void handleAsyncUpdate();

	int num_instruments() const;

private:

	std::vector<InstrumentIcon*> instruments_;
	StretchableLayoutManager instrument_layout_;
	std::mutex mutex_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstrumentViewer)
};


#endif  // INSTRUMENTVIEWER_H_INCLUDED
