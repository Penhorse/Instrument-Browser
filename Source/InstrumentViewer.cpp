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
	std::vector<Component*> components;

	for (const auto instrument : instruments_)
	{
		components.push_back(instrument);
		components.push_back(nullptr);
	}

	instrument_layout_.layOutComponents(components.data(), components.size(), 10, 10, getWidth() - 20, getHeight() - 20, false, true);
}

void InstrumentViewer::receive_instrument(const Instrument & instrument)
{
	const auto icon = new InstrumentIcon(instrument);
	instruments_.push_back(icon);

	instrument_layout_.setItemLayout((instruments_.size() - 1) * 2, icon->width(), icon->width(), icon->width());
	instrument_layout_.setItemLayout(((instruments_.size() - 1) * 2) - 1, 10, 10, 10);

	triggerAsyncUpdate();
}

void InstrumentViewer::refresh_instruments()
{
	for (const auto instrument : instruments_)
	{
		removeChildComponent(instrument);
		delete instrument;
	}

	instruments_.clear();
}

void InstrumentViewer::handleAsyncUpdate()
{
	int total_width = 0;

	for (const auto instrument : instruments_)
	{
		total_width += instrument->getWidth() + 10;
		addAndMakeVisible(instrument);
	}

	setSize(total_width, getHeight());

}
