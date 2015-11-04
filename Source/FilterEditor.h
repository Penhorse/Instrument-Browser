#ifndef FILTEREDITOR_H_INCLUDED
#define FILTEREDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class FilterEditor : public TextEditor
{

public:

	FilterEditor();
    ~FilterEditor();

    void paint (Graphics&);
    void resized();

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterEditor)
};

#endif  // FILTEREDITOR_H_INCLUDED
