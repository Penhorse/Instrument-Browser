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
/*
*/
class InstrumentIcon : public Component
{
public:
    InstrumentIcon(const Instrument & ism);
    ~InstrumentIcon();

    void paint (Graphics&);
    void resized();

private:

	Label label_;
	ImageButton button_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InstrumentIcon)
};


#endif  // INSTRUMENTICON_H_INCLUDED
