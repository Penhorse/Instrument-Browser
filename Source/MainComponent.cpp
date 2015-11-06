#include "MainComponent.h"

#include <sstream>
#include <rtw/dynamic_library.hpp>

#include "ISMSnoopWrapper.h"

//==============================================================================
MainContentComponent::MainContentComponent(const PropertiesFile::Options & options) :
	options_component_(options, this),
	options_(options),
	error_icon_(ImageFileFormat::loadFrom(BinaryData::error_png, BinaryData::error_pngSize)),
	one_row_icon_(ImageFileFormat::loadFrom(BinaryData::one_row_png, BinaryData::one_row_pngSize)),
	multirow_icon_(ImageFileFormat::loadFrom(BinaryData::multirow_png, BinaryData::multirow_pngSize)),
	error_viewer_(this),
	errors_to_display_(false)
{
	const auto cog_icon = ImageFileFormat::loadFrom(BinaryData::options_cog_png, BinaryData::options_cog_pngSize);
	const auto refresh_icon = ImageFileFormat::loadFrom(BinaryData::refresh_png, BinaryData::refresh_pngSize);
	errors_button_.setSize(24, 24);
	errors_button_.addMouseListener(this, false);
	options_button_.setImages(false, true, true, cog_icon, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	options_button_.setSize(32, 32);
	options_button_.addMouseListener(this, false);
	view_mode_button_.setImages(false, true, true, one_row_icon_, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	view_mode_button_.setSize(32, 32);
	view_mode_button_.addMouseListener(this, false);
	refresh_button_.setImages(false, true, true, refresh_icon, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
	refresh_button_.setSize(32, 32);
	refresh_button_.addMouseListener(this, false);
	filter_editor_.addListener(this);
	addAndMakeVisible(errors_button_);
	addAndMakeVisible(options_button_);
	addAndMakeVisible(view_mode_button_);
	addAndMakeVisible(refresh_button_);
	addAndMakeVisible(viewport_);
	addChildComponent(filter_editor_);

	filter_editor_.grabKeyboardFocus();

    setSize(600, 300);

	show_instruments();
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
    g.fillAll(Colour (0xff2D3134));
}

void MainContentComponent::resized()
{
	errors_button_.setTopLeftPosition(14, getHeight() - errors_button_.getHeight() - 14);
	options_button_.setTopLeftPosition(getWidth() - options_button_.getWidth() - 10, getHeight() - options_button_.getHeight() - 10);
	refresh_button_.setTopLeftPosition(options_button_.getX() - 10 - refresh_button_.getWidth(), getHeight() - refresh_button_.getHeight() - 10);
	view_mode_button_.setTopLeftPosition(refresh_button_.getX() - 10 - refresh_button_.getWidth(), getHeight() - refresh_button_.getHeight() - 10);
	filter_editor_.setBounds(getWidth() - 10 - filter_editor_.getWidth(), 10, 100, 20);
	viewport_.setBounds(10, filter_editor_.getBottom(), getWidth() - 20, options_button_.getY() - filter_editor_.getBottom() - 10);
	error_viewer_.setSize(viewport_.getWidth(), error_viewer_.getHeight());
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

	if (event.originalComponent == &view_mode_button_)
	{
		handle_view_mode_button_clicked();

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

void MainContentComponent::handle_view_mode_button_clicked()
{
	if (instrument_viewer_.get_view_mode() == InstrumentViewer::ViewMode::MultiRow)
	{
		view_mode_button_.setImages(false, true, true, one_row_icon_, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
		instrument_viewer_.set_view_mode(InstrumentViewer::ViewMode::Row);
	}
	else
	{
		view_mode_button_.setImages(false, true, true, multirow_icon_, 0.5f, Colour(), Image(), 1.0f, Colour(), Image(), 1.0f, Colour(), 0);
		instrument_viewer_.set_view_mode(InstrumentViewer::ViewMode::MultiRow);
	}
}

void MainContentComponent::handle_options_button_clicked()
{
	DialogWindow::showDialog("Options", &options_component_, 0, Colour(255, 255, 255), true, true);
}

void MainContentComponent::show_errors()
{
	set_error_button_opacities(0.8, 1.0, 1.0);
	instrument_viewer_.setVisible(false);
	error_viewer_.setVisible(true);
	viewport_.setViewedComponent(&error_viewer_, false);
	resized();
}

void MainContentComponent::show_instruments()
{
	set_error_button_opacities(0.5, 1.0, 1.0);
	instrument_viewer_.setVisible(true);
	error_viewer_.setVisible(false);
	viewport_.setViewedComponent(&instrument_viewer_, false);
	resized();
}

void MainContentComponent::reload_instruments()
{
	if(instrument_loader_)
	{
		instrument_loader_->stopThread(-1);
	}

	error_viewer_.clear_errors();

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

	set_error_button_opacities(button_normal_opacity, 1.0, 1.0);
}

void MainContentComponent::no_errors()
{
	errors_to_display_ = false;

	set_error_button_opacities(0.05, 0.05, 0.05);
}

void MainContentComponent::set_error_button_opacities(float normal, float over, float down)
{
	errors_button_.setImages(false, true, true, error_icon_, normal, Colour(), Image(), over, Colour(), Image(), down, Colour(), 0);
}

void MainContentComponent::textEditorTextChanged(TextEditor & te)
{
	if(&te == &filter_editor_)
	{
		const auto text = te.getText();

		te.setVisible(text.isNotEmpty());

		instrument_viewer_.set_filter(text);
	}
}

void MainContentComponent::textEditorEscapeKeyPressed(TextEditor & te)
{
	if(&te == &filter_editor_)
	{
		te.clear();
		te.setVisible(false);
		instrument_viewer_.set_filter(String::empty);
	}
}

bool MainContentComponent::keyPressed(const KeyPress & key)
{
	filter_editor_.grabKeyboardFocus();
	filter_editor_.keyPressed(key);

	return true;
}
