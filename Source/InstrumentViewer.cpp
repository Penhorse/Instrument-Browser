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
	for (const auto icon : icons_)
	{
		delete icon.second;
	}
}

void InstrumentViewer::paint(Graphics& g)
{
	// nothing
}

void InstrumentViewer::resized()
{
	icon_layout.layOutComponents(visible_icons_.data(), visible_icons_.size(), 10, 10, getWidth() - 20, getHeight() - 20, false, true);
}

void InstrumentViewer::receive_instrument(const Instrument & instrument)
{
	std::lock_guard<std::mutex> lock(mutex_);

	instruments_.push_back(instrument);

	const auto icon = new InstrumentIcon(instrument);

	icons_.insert({ &(instruments_.back()), icon });
	icons_to_add_.push_back(icon);

	triggerAsyncUpdate();
}

void InstrumentViewer::refresh_instruments()
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (const auto icon : icons_)
	{
		removeChildComponent(icon.second);
		delete icon.second;
	}

	icons_to_add_.clear();
	icons_.clear();
	instruments_.clear();

	triggerAsyncUpdate();
}

void InstrumentViewer::handleAsyncUpdate()
{
	std::lock_guard<std::mutex> lock(mutex_);

	for (const auto icon : icons_to_add_)
	{
		addChildComponent(icon);
	}

	icons_to_add_.clear();

	apply_filter();
}

int InstrumentViewer::num_instruments() const
{
	return instruments_.size();
}

void InstrumentViewer::set_filter(const String & filter)
{
	filter_ = filter;

	triggerAsyncUpdate();
}

void InstrumentViewer::apply_filter()
{
	visible_icons_.clear();

	int total_width = 0;

	if(filter_.isEmpty())
	{
		for(const auto icon : icons_)
		{
			push_visible_icon(icon.second);

			total_width += icon.second->getWidth() + 10;
		}
	}
	else
	{
		for(const auto & instrument : instruments_)
		{
			const auto icon = icons_[&instrument];

			if(instrument.matches_filter(filter_.toStdString()))
			{
				push_visible_icon(icon);

				total_width += icon->getWidth() + 10;
			}
			else
			{
				icon->setVisible(false);
			}
		}
	}

	setSize(total_width, getHeight());
}

void InstrumentViewer::push_visible_icon(InstrumentIcon * icon)
{
	const auto index = (visible_icons_.size() / 2) + 1;

	icon->setVisible(true);

	visible_icons_.push_back(icon);
	visible_icons_.push_back(nullptr);

	icon_layout.setItemLayout((index - 1) * 2, icon->width(), icon->width(), icon->width());
	icon_layout.setItemLayout(((index - 1) * 2) - 1, 10, 10, 10);
}
