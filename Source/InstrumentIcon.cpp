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
	Image image();

	//button_.setImages(false, true, true, )
	label_.setJustificationType(Justification::centredBottom);
	addAndMakeVisible(label_);

	setSize(100, 100);
}

InstrumentIcon::~InstrumentIcon()
{
}

void InstrumentIcon::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void InstrumentIcon::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

	label_.setBounds(0, 0, getWidth(), getHeight());
}
