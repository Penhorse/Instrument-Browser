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

	enum class ViewMode
	{
		Row,
		MultiRow,
	};

    InstrumentViewer();
    ~InstrumentViewer();

    void paint(Graphics&) override;
    void resized() override;
	void parentSizeChanged() override;

	void receive_instrument(const Instrument & instrument) override;
	void refresh_instruments() override;

	void handleAsyncUpdate();

	int num_instruments() const;

	void set_filter(const String & filter);

	void set_view_mode(ViewMode view_mode);
	ViewMode get_view_mode() const;

private:

	void apply_filter();
	void apply_view_mode();
	void apply_row_view_mode();
	void apply_multirow_view_mode();

	struct IconRow
	{
		int height;
		std::vector<InstrumentIcon*> icons;
	};

	std::deque<Instrument> instruments_;
	std::map<const Instrument*, InstrumentIcon*> icons_;
	std::vector<InstrumentIcon*> visible_icons_;
	std::deque<InstrumentIcon*> icons_to_add_;
	std::deque<IconRow> icon_rows_;
	String filter_;
	ViewMode view_mode_;
	int calculated_width_;
	int calculated_height_;
	int icons_per_row_;
	std::mutex mutex_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstrumentViewer)
};

#endif  // INSTRUMENTVIEWER_H_INCLUDED
