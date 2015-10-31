/*
  ==============================================================================

    InstrumentViewer.cpp
    Created: 31 Oct 2015 5:47:33pm
    Author:  Chris

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "InstrumentViewer.h"

//==============================================================================
InstrumentViewer::InstrumentViewer()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

InstrumentViewer::~InstrumentViewer()
{
	for (const auto instrument : instruments_)
	{
		delete instrument;
	}
}

void InstrumentViewer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

}

void InstrumentViewer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void InstrumentViewer::receive_instrument(const Instrument & instrument)
{
	const auto icon = new InstrumentIcon(instrument);
	instruments_.push_back(icon);

	addAndMakeVisible(icon);

	instrument_layout_.setItemLayout((instruments_.size() - 1) * 2, 100, 100, 100);
	instrument_layout_.setItemLayout(((instruments_.size() - 1) * 2) - 1, 10, 10, 10);

	std::vector<Component*> components;

	int total_width = 0;

	for (const auto instrument : instruments_)
	{
		components.push_back(instrument);
		components.push_back(nullptr);

		total_width += 110;
	}

	setSize(total_width, getHeight());

	instrument_layout_.layOutComponents(components.data(), components.size(), 10, 10, getWidth() - 20, getHeight() - 20, false, true);
}

void InstrumentViewer::refresh_instruments()
{
	// TODO
}
