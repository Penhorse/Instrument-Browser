#ifndef ERRORVIEWER_H_INCLUDED
#define ERRORVIEWER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "ErrorReceiver.h"

//==============================================================================
class ErrorViewer : public Component, public ErrorReceiver, public AsyncUpdater
{

public:

	class Listener
	{

	public:

		virtual void have_errors() {}
		virtual void no_errors() {}

	};

    ErrorViewer(Listener * listener);
    ~ErrorViewer();

    void paint (Graphics&);
    void resized();

	void receive_error(const std::string & what) override;
	void clear_errors() override;

	void handleAsyncUpdate();

private:

	String error_log_;
	TextEditor error_log_view_;
	Listener * listener_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ErrorViewer)
};


#endif  // ERRORVIEWER_H_INCLUDED
