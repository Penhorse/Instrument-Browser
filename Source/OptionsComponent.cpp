/*
  ==============================================================================

    OptionsComponent.cpp
    Created: 30 Oct 2015 11:03:10pm
    Author:  Chris

  ==============================================================================
*/

#include "OptionsComponent.h"
#include "InstrumentLoader.h"

//==============================================================================
OptionsComponent::OptionsComponent(const PropertiesFile::Options & options, MessageListener * listener) :
	directories_label_(String::empty, "Library Directories (one per line):"),
	apply_button_("Apply"),
	options_(options),
	listener_(listener)
{
	ApplicationProperties props;

	props.setStorageParameters(options);

	const auto user_settings = props.getUserSettings();

	directories_editor_.setMultiLine(true, false);
	directories_editor_.setReturnKeyStartsNewLine(true);
	directories_editor_.setText(user_settings->getValue("directories"));

	buttons_layout_.setItemLayout(0, 0, -1, -1);
	buttons_layout_.setItemLayout(1, 100, 100, 100);

	apply_button_.addMouseListener(this, false);

	addAndMakeVisible(directories_label_);
	addAndMakeVisible(apply_button_);
	addAndMakeVisible(directories_editor_);

	setSize(800, 200);
}

OptionsComponent::~OptionsComponent()
{
}

void OptionsComponent::paint (Graphics& g)
{
    g.fillAll (Colours::white);   // clear the background
}

void OptionsComponent::resized()
{
	Component* button_components[] = { 0, &apply_button_ };

	buttons_layout_.layOutComponents(button_components, 2, 10, getHeight() - 30, getWidth() - 20, 20, false, true);

	directories_label_.setBounds(10, 10, getWidth() - 20, directories_label_.getFont().getHeight());

	directories_label_.setJustificationType(Justification::centredLeft);

	directories_editor_.setBounds(10, directories_label_.getBottom() + 10, getWidth() - 20, apply_button_.getY() - 20 - directories_label_.getBottom());
}

void OptionsComponent::mouseDown(const MouseEvent &event)
{
	if (event.originalComponent == &apply_button_)
	{
		ApplicationProperties props;

		props.setStorageParameters(options_);

		const auto user_settings = props.getUserSettings();

		const auto df = options_.getDefaultFile();

		user_settings->setValue("directories", directories_editor_.getText());

		user_settings->saveIfNeeded();

		listener_->postMessage(new Message(Message::Type::OptionsWereChanged));

		findParentComponentOfClass<DialogWindow>()->exitModalState(0);
	}
}
