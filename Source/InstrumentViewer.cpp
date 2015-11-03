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
	// nothing
}

InstrumentViewer::~InstrumentViewer()
{
	for (const auto instrument : instruments_)
	{
		delete instrument;
	}
}

void InstrumentViewer::paint(Graphics& g)
{
	// nothing
}

void InstrumentViewer::resized()
{
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
	std::lock_guard<std::mutex> lock(mutex_);

	const auto icon = new InstrumentIcon(instrument);
	instruments_.push_back(icon);

	instrument_layout_.setItemLayout((instruments_.size() - 1) * 2, icon->width(), icon->width(), icon->width());
	instrument_layout_.setItemLayout(((instruments_.size() - 1) * 2) - 1, 10, 10, 10);

	triggerAsyncUpdate();
}

void InstrumentViewer::refresh_instruments()
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (const auto instrument : instruments_)
	{
		removeChildComponent(instrument);
		delete instrument;
	}

	instruments_.clear();
}

void InstrumentViewer::handleAsyncUpdate()
{
	std::lock_guard<std::mutex> lock(mutex_);

	int total_width = 0;

	for (const auto instrument : instruments_)
	{
		total_width += instrument->getWidth() + 10;
		addAndMakeVisible(instrument);
	}

	setSize(total_width, getHeight());
}
