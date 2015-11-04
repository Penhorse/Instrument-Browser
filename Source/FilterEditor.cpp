#include "FilterEditor.h"

FilterEditor::FilterEditor()
{
	setColour(TextEditor::ColourIds::backgroundColourId, Colours::white);
	setColour(TextEditor::ColourIds::outlineColourId, Colours::yellow);
	setColour(TextEditor::ColourIds::focusedOutlineColourId, Colours::yellow);
}

FilterEditor::~FilterEditor()
{
}

void FilterEditor::paint(Graphics& g)
{
	TextEditor::paint(g);
}

void FilterEditor::resized()
{
}
