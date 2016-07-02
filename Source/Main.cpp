#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

static PropertiesFile::Options make_options()
{
	PropertiesFile::Options result;

	result.applicationName = "Instrument Browser";
	result.folderName = "InstrumentBrowser";
	result.filenameSuffix = ".properties";
    result.osxLibrarySubFolder = "Application Support";

	return result;
}

//==============================================================================
class InstrumentBrowserApplication  : public JUCEApplication
{
public:
    //==============================================================================
    InstrumentBrowserApplication() : options_(make_options()) {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
		mainWindow = new MainWindow(getApplicationName(), options_);
    }

    void shutdown() override
    {
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
    }

    //==============================================================================
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name, const PropertiesFile::Options & options) :
			DocumentWindow (name, Colours::lightgrey, DocumentWindow::allButtons),
			main_content_(options)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (&main_content_, true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
			setResizable(true, true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

		bool keyPressed(const KeyPress & key) override
		{
			DocumentWindow::keyPressed(key);

			return main_content_.keyPressed(key);
		}

    private:

		MainContentComponent main_content_;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:

	PropertiesFile::Options options_;
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (InstrumentBrowserApplication)
