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
	label_(String::empty, ism.name),
	button_(File(ism.path))
{
	Image image;

	if (!ism.icon.bytes.empty())
	{
		image = Image(Image::PixelFormat::ARGB, ism.icon.width, ism.icon.height, true);

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
	}
	else
	{
		image = ImageFileFormat::loadFrom(BinaryData::instrument_png, BinaryData::instrument_pngSize);
	}

	button_.setImages(true, false, false, image, 1.0f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour());
	label_.setJustificationType(Justification::centredTop);
	label_.setColour(Label::ColourIds::textColourId, Colours::white);
	addAndMakeVisible(label_);
	addAndMakeVisible(button_);

	width_ = std::max(image.getWidth(), 100);

	setSize(width_, image.getHeight());

	layout_.setItemLayout(0, 0, -1, -.25);
	layout_.setItemLayout(1, image.getHeight(), image.getHeight(), image.getHeight());
	layout_.setItemLayout(2, 10, 10, 10);
	layout_.setItemLayout(3, 0, -1, -.25);
}

InstrumentIcon::~InstrumentIcon()
{
}

void InstrumentIcon::paint (Graphics& g)
{
   // g.setColour (Colours::grey);
   // g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
	g.setFillType(Colour());
	g.drawRect(button_.getBounds(), 10.0f);
}

void InstrumentIcon::resized()
{
	Component * components[] = { 0, &button_, 0, &label_ };

	layout_.layOutComponents(components, 4, 0, 0, width_, getHeight(), true, true);
}
