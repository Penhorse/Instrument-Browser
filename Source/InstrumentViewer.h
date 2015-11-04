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

#include <deque>
#include <map>
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

	void set_filter(const String & filter);
	void apply_filter();

private:

	void push_visible_icon(InstrumentIcon * icon);

	std::deque<Instrument> instruments_;
	std::map<const Instrument*, InstrumentIcon*> icons_;
	std::vector<Component*> visible_icons_;
	std::deque<InstrumentIcon*> icons_to_add_;
	StretchableLayoutManager icon_layout;
	String filter_;
	std::mutex mutex_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstrumentViewer)
};

#endif  // INSTRUMENTVIEWER_H_INCLUDED
