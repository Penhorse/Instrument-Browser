/*
  ==============================================================================

    InstrumentIcon.cpp
    Created: 31 Oct 2015 5:47:49pm
    Author:  Chris

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "InstrumentIcon.h"

//==============================================================================
InstrumentIcon::InstrumentIcon(const Instrument & ism) :
	label_(String::empty, ism.path)
{
	Image image(Image::PixelFormat::ARGB, ism.icon.width, ism.icon.height, true);

	for (int row = 1; row < ism.icon.height; row++)
	{
		for (int col = 0; col < ism.icon.width; col++)
		{
			const auto b = ism.icon.bytes[((ism.icon.width * 4) * row) + (4 * col) + 0];
			const auto g = ism.icon.bytes[((ism.icon.width * 4) * row) + (4 * col) + 1];
			const auto r = ism.icon.bytes[((ism.icon.width * 4) * row) + (4 * col) + 2];
			//const auto a = ism.icon.bytes[(ism.icon.width * row) + (4 * col) + 3];

			image.setPixelAt(col, ism.icon.height - row, Colour(r, g, b));
		}
	}
	button_.setImages(true, false, false, image, 1.0f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour());
	label_.setJustificationType(Justification::centredBottom);
	addAndMakeVisible(label_);
	addAndMakeVisible(button_);

	setSize(ism.icon.width, ism.icon.height);

	layout_.setItemLayout(0, ism.icon.height, ism.icon.height, ism.icon.height);
	layout_.setItemLayout(1, 10, 10, 10);
	layout_.setItemLayout(2, -1, -1, -1);
}

InstrumentIcon::~InstrumentIcon()
{
}

void InstrumentIcon::paint (Graphics& g)
{
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void InstrumentIcon::resized()
{
	Component * components[] = { &button_, 0, &label_ };

	layout_.layOutComponents(components, 3, 0, 0, getWidth(), getHeight(), true, true);
}
