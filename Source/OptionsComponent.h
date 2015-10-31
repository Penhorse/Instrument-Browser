/*
  ==============================================================================

    OptionsComponent.h
    Created: 30 Oct 2015 11:03:10pm
    Author:  Chris

  ==============================================================================
*/

#ifndef OPTIONSCOMPONENT_H_INCLUDED
#define OPTIONSCOMPONENT_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class OptionsComponent : public Component, public MouseListener
{
public:
	OptionsComponent(const PropertiesFile::Options & options, MessageListener * listener);
    ~OptionsComponent();

    void paint (Graphics&);
    void resized();

	void mouseDown(const MouseEvent &event);

	struct Message : public juce::Message
	{
		enum class Type
		{
			OptionsWereChanged
		};

		Message(Type t) : type(t) {}

		Type type;
	};

private:

	Label directories_label_;
	StretchableLayoutManager buttons_layout_;
	PropertiesFile::Options options_;
	TextButton apply_button_;
	TextEditor directories_editor_;
	MessageListener * listener_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OptionsComponent)
};


#endif  // OPTIONSCOMPONENT_H_INCLUDED
