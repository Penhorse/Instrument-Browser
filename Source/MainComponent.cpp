#include "MainComponent.h"

#include <sstream>
#include <rtw/dynamic_library.hpp>

#include "ISMSnoopWrapper.h"

//==============================================================================
MainContentComponent::MainContentComponent(const PropertiesFile::Options & options) :
	options_component_(options, this),
	options_(options)
{
	const auto cog_image = ImageFileFormat::loadFrom(BinaryData::options_cog_png, BinaryData::options_cog_pngSize);
	options_button_.setImages(true, true, true, cog_image, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	options_button_.setSize(32, 32);
	options_button_.addMouseListener(this, false);
	refresh_button_.setImages(true, true, true, cog_image, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	refresh_button_.setSize(32, 32);
	refresh_button_.addMouseListener(this, false);
	addAndMakeVisible(options_button_);
	addAndMakeVisible(refresh_button_);
	addAndMakeVisible(instrument_viewer_);
	addAndMakeVisible(viewport_);
	viewport_.setViewedComponent(&instrument_viewer_, false);
    setSize(600, 400);

	reload_instruments();
}

MainContentComponent::~MainContentComponent()
{
	if(instrument_loader_)
	{
		instrument_loader_->stopThread(500);
	}
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll(Colour (0xff001F36));

	if (error_message_.isNotEmpty())
	{
		g.setFont(Font (16.0f));
		g.setColour(Colours::white);
		g.drawText(error_message_, getLocalBounds(), Justification::centred, true);
	}
}

void MainContentComponent::resized()
{
	options_button_.setTopLeftPosition(getWidth() - options_button_.getWidth() - 10, getHeight() - options_button_.getHeight() - 10);
	refresh_button_.setTopLeftPosition(options_button_.getX() - 10 - refresh_button_.getWidth(), getHeight() - refresh_button_.getHeight() - 10);
	viewport_.setBounds(10, 10, getWidth() - 20, options_button_.getY() - 20);
	instrument_viewer_.setSize(instrument_viewer_.getWidth(), viewport_.getHeight() - 20);
}

void MainContentComponent::mouseDown(const MouseEvent &event)
{
	if (event.originalComponent == &refresh_button_)
	{
		reload_instruments();
		return;
	}

	if (event.originalComponent == &options_button_)
	{
		DialogWindow::showModalDialog("Options", &options_component_, 0, Colour(255, 255, 255), true, true);
		return;
	}
}

void MainContentComponent::clear_error_message()
{
	viewport_.setVisible(true);
	error_message_.clear();
	repaint();
}

void MainContentComponent::set_error_message(const String & error_message)
{
	viewport_.setVisible(false);
	error_message_ = error_message;
	repaint();
}

void MainContentComponent::reload_instruments()
{
	clear_error_message();

	instrument_viewer_.refresh_instruments();

	const auto ismsnoop = std::make_shared<ISMSnoopWrapper>();

	if (!ismsnoop->load())
	{
		set_error_message("Failed to load " + ismsnoop->library_filename() + ".");
		return;
	}

	ApplicationProperties props;

	props.setStorageParameters(options_);

	const auto user_settings = props.getUserSettings();

	const auto directories_string = user_settings->getValue("directories");

	if (directories_string.isEmpty())
	{
		set_error_message("No library directories specified.");
		return;
	}

	StringArray directories;

	std::stringstream ss(directories_string.toStdString());
	std::string directory;

	while (std::getline(ss, directory, '\n'))
	{
		directories.add(directory);
	}
	
	if(instrument_loader_)
	{
		instrument_loader_->stopThread(500);
	}

	instrument_loader_ = std::unique_ptr<InstrumentLoader>(new InstrumentLoader(directories, ismsnoop, &instrument_viewer_));

	instrument_loader_->startThread();
}

void MainContentComponent::handle_options_changed()
{
	reload_instruments();
}

void MainContentComponent::handleMessage(const Message & base_message)
{
	const auto message = dynamic_cast<const OptionsComponent::Message*>(&base_message);

	switch (message->type)
	{
		case OptionsComponent::Message::Type::OptionsWereChanged:
		{
			handle_options_changed();
			break;
		}

		default:
		{
			break;
		}
	}
}
