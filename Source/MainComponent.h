#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

#include "ErrorViewer.h"
#include "InstrumentViewer.h"
#include "OptionsComponent.h"
#include "InstrumentLoader.h"

//==============================================================================
class MainContentComponent   : public Component, public MessageListener, public ErrorViewer::Listener
{
public:
    //==============================================================================
	MainContentComponent(const PropertiesFile::Options & options);
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();

	void mouseDown(const MouseEvent &event);

	void reload_instruments();
	void handleMessage(const Message & message) override;

	void have_errors() override;
	void no_errors() override;

private:

	void show_errors();
	void show_instruments();
	void handle_options_changed();
	void handle_errors_button_clicked();
	void handle_refresh_button_clicked();
	void handle_options_button_clicked();

	OptionsComponent options_component_;
	ImageButton errors_button_;
	ImageButton refresh_button_;
	ImageButton options_button_;
	PropertiesFile::Options options_;
	Image error_icon_;
	Viewport viewport_;
	std::unique_ptr<InstrumentLoader> instrument_loader_;
	InstrumentViewer instrument_viewer_;
	ErrorViewer error_viewer_;
	bool errors_to_display_;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
