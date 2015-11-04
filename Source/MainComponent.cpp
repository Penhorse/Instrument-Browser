#include "MainComponent.h"

#include <sstream>
#include <rtw/dynamic_library.hpp>

#include "ISMSnoopWrapper.h"

//==============================================================================
MainContentComponent::MainContentComponent(const PropertiesFile::Options & options) :
	options_component_(options, this),
	options_(options),
	error_icon_(ImageFileFormat::loadFrom(BinaryData::error_png, BinaryData::error_pngSize)),
	error_viewer_(this),
	errors_to_display_(false)
{
	const auto cog_image = ImageFileFormat::loadFrom(BinaryData::options_cog_png, BinaryData::options_cog_pngSize);
	const auto refresh_image = ImageFileFormat::loadFrom(BinaryData::refresh_png, BinaryData::refresh_pngSize);
	errors_button_.setSize(24, 24);
	errors_button_.addMouseListener(this, false);
	options_button_.setImages(true, true, true, cog_image, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	options_button_.setSize(32, 32);
	options_button_.addMouseListener(this, false);
	refresh_button_.setImages(true, true, true, refresh_image, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	refresh_button_.setSize(32, 32);
	refresh_button_.addMouseListener(this, false);
	filter_editor_.addListener(this);
	addAndMakeVisible(errors_button_);
	addAndMakeVisible(options_button_);
	addAndMakeVisible(refresh_button_);
	addAndMakeVisible(viewport_);
	addAndMakeVisible(filter_editor_);
	show_instruments();
    setSize(600, 300);

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
}

void MainContentComponent::resized()
{
	errors_button_.setTopLeftPosition(14, getHeight() - errors_button_.getHeight() - 14);
	options_button_.setTopLeftPosition(getWidth() - options_button_.getWidth() - 10, getHeight() - options_button_.getHeight() - 10);
	refresh_button_.setTopLeftPosition(options_button_.getX() - 10 - refresh_button_.getWidth(), getHeight() - refresh_button_.getHeight() - 10);
	viewport_.setBounds(10, 10, getWidth() - 20, options_button_.getY() - 20);
	instrument_viewer_.setSize(instrument_viewer_.getWidth(), viewport_.getHeight() - 20);
	error_viewer_.setSize(viewport_.getWidth(), error_viewer_.getHeight());
	filter_editor_.setBounds(getWidth() - 10 - filter_editor_.getWidth(), 10, 100, 20);
}

void MainContentComponent::mouseDown(const MouseEvent &event)
{
	if (event.originalComponent == &errors_button_)
	{
		handle_errors_button_clicked();

		return;
	}

	if (event.originalComponent == &refresh_button_)
	{
		handle_refresh_button_clicked();

		return;
	}

	if (event.originalComponent == &options_button_)
	{
		handle_options_button_clicked();

		return;
	}
}

void MainContentComponent::handle_errors_button_clicked()
{
	if(viewport_.getViewedComponent() == &error_viewer_)
	{
		if (instrument_viewer_.num_instruments() > 0)
		{
			show_instruments();
		}
	}
	else if(viewport_.getViewedComponent() == &instrument_viewer_)
	{
		if (errors_to_display_)
		{
			show_errors();
		}
	}
}

void MainContentComponent::handle_refresh_button_clicked()
{
	reload_instruments();
}

void MainContentComponent::handle_options_button_clicked()
{
	DialogWindow::showDialog("Options", &options_component_, 0, Colour(255, 255, 255), true, true);
}

void MainContentComponent::show_errors()
{
	errors_button_.setImages(false, true, true, error_icon_, 0.8f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	instrument_viewer_.setVisible(false);
	error_viewer_.setVisible(true);
	viewport_.setViewedComponent(&error_viewer_, false);
	resized();
}

void MainContentComponent::show_instruments()
{
	errors_button_.setImages(false, true, true, error_icon_, 0.5, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	instrument_viewer_.setVisible(true);
	error_viewer_.setVisible(false);
	viewport_.setViewedComponent(&instrument_viewer_, false);
	resized();
}

void MainContentComponent::reload_instruments()
{
	error_viewer_.clear_errors();

	if(instrument_loader_)
	{
		instrument_loader_->stopThread(500);
	}

	instrument_viewer_.refresh_instruments();

	const auto ismsnoop = std::make_shared<ISMSnoopWrapper>();

	if (!ismsnoop->load())
	{
		std::stringstream ss;

		ss << "Failed to load " + ismsnoop->library_filename() + ".";

		error_viewer_.receive_error(ss.str());

		show_errors();

		return;
	}

	ApplicationProperties props;

	props.setStorageParameters(options_);

	const auto user_settings = props.getUserSettings();

	const auto directories_string = user_settings->getValue("directories");

	if (directories_string.isEmpty())
	{
		error_viewer_.receive_error("No library directories specified.");

		show_errors();

		return;
	}

	StringArray directories;

	std::stringstream ss(directories_string.toStdString());
	std::string directory;

	while (std::getline(ss, directory, '\n'))
	{
		directories.add(directory);
	}
	
	instrument_loader_ =
		std::unique_ptr<InstrumentLoader>(
				new InstrumentLoader(
					directories,
					ismsnoop,
					&instrument_viewer_,
					&error_viewer_));

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

void MainContentComponent::have_errors()
{
	errors_to_display_ = true;

	const auto button_normal_opacity =
		viewport_.getViewedComponent() == &error_viewer_
			? 0.8
			: 0.5;

	errors_button_.setImages(false, true, true, error_icon_, button_normal_opacity, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
}

void MainContentComponent::no_errors()
{
	errors_to_display_ = false;

	errors_button_.setImages(false, true, true, error_icon_, 0.05, Colour(), Image(), 0.1f, Colour(), Image(), 0.1f, Colour(), 0);
}

void MainContentComponent::textEditorTextChanged(TextEditor & te)
{
}
