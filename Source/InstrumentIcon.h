/*
  ==============================================================================

    InstrumentIcon.h
    Created: 31 Oct 2015 5:47:49pm
    Author:  Chris

  ==============================================================================
*/

#ifndef INSTRUMENTICON_H_INCLUDED
#define INSTRUMENTICON_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "Instrument.h"

//==============================================================================
class InstrumentButton : public ImageButton
{

public:

	InstrumentButton(File file) :
		file_(file)
	{
		// nothing
	}

	void clicked() override
	{
		file_.startAsProcess();
	}

private:

	File file_;
};

class InstrumentIcon : public Component
{
public:
    InstrumentIcon(const Instrument & ism);
    ~InstrumentIcon();

    void paint (Graphics&);
    void resized();

	int width() const
	{
		return width_;
	}

private:

	Label label_;
	InstrumentButton button_;
	StretchableLayoutManager layout_;
	int width_;
	File instrument_file_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstrumentIcon)
};


#endif  // INSTRUMENTICON_H_INCLUDED
