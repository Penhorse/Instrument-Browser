#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

#include "ErrorViewer.h"
#include "InstrumentViewer.h"
#include "OptionsComponent.h"
#include "InstrumentLoader.h"
#include "FilterEditor.h"

//==============================================================================
class MainContentComponent :
	public Component,
	public MessageListener,
	public ErrorViewer::Listener,
	public FilterEditor::Listener
{
public:
    //==============================================================================
	MainContentComponent(const PropertiesFile::Options & options);
    ~MainContentComponent();

    void paint (Graphics&) override;
    void resized() override;

	void mouseDown(const MouseEvent &event);

	void reload_instruments();
	void handleMessage(const Message & message) override;

	void have_errors() override;
	void no_errors() override;

	void textEditorTextChanged(TextEditor & te) override;
	void textEditorEscapeKeyPressed(TextEditor & te) override;
	bool keyPressed(const KeyPress & key) override;

private:

	void show_errors();
	void show_instruments();
	void handle_options_changed();
	void handle_errors_button_clicked();
	void handle_refresh_button_clicked();
	void handle_options_button_clicked();
	void handle_view_mode_button_clicked();
	void set_view_mode(InstrumentViewer::ViewMode view_mode);
	void set_error_button_opacities(float normal, float over, float down);
	void add_and_make_visible(const std::deque<Component*> & components);
	void report_fatal_error(const std::string & error);
	StringArray get_directories() const;

	OptionsComponent options_component_;
	ImageButton errors_button_;
	ImageButton refresh_button_;
	ImageButton options_button_;
	ImageButton view_mode_button_;;
	PropertiesFile::Options options_;
	Image error_icon_;
	Image one_row_icon_;
	Image multirow_icon_;
	Viewport viewport_;
	std::unique_ptr<InstrumentLoader> instrument_loader_;
	InstrumentViewer instrument_viewer_;
	ErrorViewer error_viewer_;
	bool errors_to_display_;
	FilterEditor filter_editor_;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
