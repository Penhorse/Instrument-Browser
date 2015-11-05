/*
  ==============================================================================

    InstrumentViewer.cpp
    Created: 31 Oct 2015 5:47:33pm
    Author:  Chris

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

#include <rtw/bits.hpp>

#include "InstrumentViewer.h"

//==============================================================================
InstrumentViewer::InstrumentViewer() :
	view_mode_(ViewMode::Row)
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

void InstrumentViewer::parentSizeChanged()
{
	apply_view_mode();
	setSize(calculated_width_, calculated_height_);
}

void InstrumentViewer::resized()
{
	int row_y = 10;

	for(auto & row : icon_rows_)
	{
		StretchableLayoutManager layout;

		int index = 1;

		std::vector<Component*> components;

		for(const auto icon : row.icons)
		{
			layout.setItemLayout((index - 1) * 2, icon->width(), icon->width(), icon->width());
			layout.setItemLayout(((index - 1) * 2) - 1, 10, 10, 10);

			components.push_back(icon);
			components.push_back(nullptr);

			index++;
		}

		layout.layOutComponents(components.data(), components.size(), 10, row_y, getWidth() - 20, row.height, false, true);

		row_y += row.height + 10;
	}
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
	apply_view_mode();

	setSize(calculated_width_, calculated_height_);
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

	if(filter_.isEmpty())
	{
		for (const auto & instrument : instruments_)
		{
			const auto icon = icons_[&instrument];

			icon->setVisible(true);

			visible_icons_.push_back(icon);
		}
	}
	else
	{
		for(const auto & instrument : instruments_)
		{
			const auto icon = icons_[&instrument];

			if(instrument.matches_filter(filter_.toStdString()))
			{
				icon->setVisible(true);

				visible_icons_.push_back(icon);
			}
			else
			{
				icon->setVisible(false);
			}
		}
	}
}

void InstrumentViewer::apply_view_mode()
{
	switch(view_mode_)
	{
		case ViewMode::Row:
		{
			apply_row_view_mode();

			return;
		}

		case ViewMode::MultiRow:
		{
			apply_multirow_view_mode();

			return;
		}
	}
}

void InstrumentViewer::apply_row_view_mode()
{
	calculated_width_ = 0;
	calculated_height_ = getParentHeight() - 20;

	icon_rows_.clear();

	IconRow only_row;

	only_row.height = 0;

	int index = 1;

	for(const auto icon : visible_icons_)
	{
		calculated_width_ += icon->width() + 10;

		only_row.icons.push_back(icon);

		if(icon->height() > only_row.height)
		{
			only_row.height = icon->height();
		}

		index++;
	}

	icon_rows_.push_back(only_row);
}

void InstrumentViewer::apply_multirow_view_mode()
{
	calculated_width_ = getParentWidth();
	calculated_height_ = 0;

	icon_rows_.clear();

	int row_width = 0;

	IconRow row;

	row.height = 0;

	for(const auto icon : visible_icons_)
	{
		if(row_width + icon->width() + 10 > calculated_width_)
		{
			icon_rows_.push_back(row);
			calculated_height_ += row.height + 10;
			row_width = 0;
			row.height = 0;
			row.icons.clear();
		}

		row.icons.push_back(icon);

		if(icon->height() > row.height)
		{
			row.height = icon->height();
		}

		row_width += icon->width() + 10;
	}

	icon_rows_.push_back(row);
	calculated_height_ += row.height + 10;
}

void InstrumentViewer::set_view_mode(ViewMode view_mode)
{
	view_mode_ = view_mode;

	triggerAsyncUpdate();
}

auto InstrumentViewer::get_view_mode() const -> ViewMode
{
	return view_mode_;
}
