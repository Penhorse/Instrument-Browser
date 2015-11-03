#include "ErrorViewer.h"

ErrorViewer::ErrorViewer(Listener * listener) :
	listener_(listener)
{
	error_log_view_.setReadOnly(true);
	error_log_view_.setCaretVisible(false);
	error_log_view_.setMultiLine(true);
	error_log_view_.setBorder(BorderSize<int>());
	error_log_view_.setColour(TextEditor::ColourIds::backgroundColourId, Colour());
	error_log_view_.setColour(TextEditor::ColourIds::shadowColourId, Colour());
	error_log_view_.setColour(TextEditor::ColourIds::highlightColourId, Colours::white);
	error_log_view_.setColour(TextEditor::ColourIds::highlightedTextColourId, Colours::black);
	error_log_view_.setColour(TextEditor::ColourIds::textColourId, Colours::white);

	addAndMakeVisible(error_log_view_);
}

ErrorViewer::~ErrorViewer()
{
	// nothing
}

void ErrorViewer::paint(Graphics& g)
{
	// nothing
}

void ErrorViewer::resized()
{
	error_log_view_.setBounds(0, 0, getWidth(), getHeight());
}

void ErrorViewer::receive_error(const std::string & what)
{
	if(!error_log_.isEmpty())
	{
		error_log_ += '\n';
	}

	error_log_ += what;

	triggerAsyncUpdate();
}

void ErrorViewer::clear_errors()
{
	error_log_.clear();

	triggerAsyncUpdate();
}

void ErrorViewer::handleAsyncUpdate()
{
	error_log_view_.setText(error_log_);

	setSize(getWidth(), error_log_view_.getTextHeight());

	if (error_log_.isNotEmpty())
	{
		listener_->have_errors();
	}
	else
	{
		listener_->no_errors();
	}
}
