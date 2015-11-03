/*
  ==============================================================================

    InstrumentLoader.cpp
    Created: 31 Oct 2015 2:36:54pm
    Author:  Chris

  ==============================================================================
*/

#include "InstrumentLoader.h"

#include "ErrorReceiver.h"
#include "InstrumentReceiver.h"

#include <chrono>
#include <sstream>
#include <thread>

InstrumentLoader::InstrumentLoader(
		const StringArray & directories,
		std::shared_ptr<ISMSnoopWrapper> ismsnoop,
		InstrumentReceiver * instrument_receiver,
		ErrorReceiver * error_receiver) :
	Thread("InstrumentLoader"),
	directories_(directories),
	ismsnoop_(ismsnoop),
	instrument_receiver_(instrument_receiver),
	error_receiver_(error_receiver)
{
	// nothing
}

static const String EXCLUDED_DIRECTORIES[] =
{
	"__MACOSX"
};

static bool excluded(const File & dir)
{
	for(const auto & XD : EXCLUDED_DIRECTORIES)
	{
		const auto path = dir.getFullPathName();

		if(path.contains(String("/") + XD) || path.contains(String("\\") + XD))
		{
			return true;
		}
	}

	return false;
}

void InstrumentLoader::run()
{
	int num_instruments_found = 0;

	for (const auto & dir : directories_)
	{
		if (!File::isAbsolutePath(dir))
		{
			std::stringstream ss;

			ss << "'" << dir << "' is not an absolute path.";

			error_receiver_->receive_error(ss.str());
		}
		else
		{
			File dir_file(dir);
			num_instruments_found += load_directory(dir_file);

			if(threadShouldExit())
			{
				return;
			}
		}
	}

	if(num_instruments_found == 0)
	{
		error_receiver_->receive_error("No instruments found in the specified directories.");
	}
}

int InstrumentLoader::try_load_directory(const File & dir) const
{
	return dir.isDirectory() ? load_directory(dir) : 0;
}

//
// returns:
//	the number of instruments found in the directory
//
int InstrumentLoader::load_directory(const File & dir) const
{
	int result = 0;

	DirectoryIterator directory_iterator(dir, true, "*.ism", File::TypesOfFileToFind::findFiles);

	while (directory_iterator.next())
	{
		if (threadShouldExit())
		{
			return result;
		}

		const auto file = directory_iterator.getFile();

		if (!excluded(file))
		{
			load_file(file);
			result++;
		}
	}

	return result;
}

void InstrumentLoader::load_file(const File & file) const
{
	Instrument instrument;

	instrument.icon.width = 0;
	instrument.icon.height = 0;

	instrument.path = file.getFullPathName().toStdString();

	const auto ism = ismsnoop_->open(instrument.path.c_str());

	if (!ism)
	{
		std::stringstream ss;

		ss << "Failed to open '" << instrument.path << "' for some reason.";

		error_receiver_->receive_error(ss.str());
	}
	else
	{
		instrument.name = get_name(ism);
		instrument.icon = get_icon(ism);

		//
		// at least the name or the panel icon is required, otherwise an error
		// is reported
		//
		if(instrument.name.empty() && instrument.icon.bytes.size() == 0)
		{
			std::stringstream ss;

			ss << "Failed to read '" << instrument.path << "'.";

			error_receiver_->receive_error(ss.str());
		}
		else
		{
			instrument_receiver_->receive_instrument(instrument);
		}
	}

	ismsnoop_->close(ism);
}

std::string InstrumentLoader::get_name(ISMSnoopInstrument * ism) const
{
	int name_length = 0;

	ismsnoop_->get_name(ism, nullptr, &name_length);

	if (name_length > 0)
	{
		std::vector<char> buffer(name_length + 1);

		ismsnoop_->get_name(ism, buffer.data(), nullptr);

		return std::string(buffer.begin(), buffer.end());
	}

	return std::string();
}

Instrument::Icon InstrumentLoader::get_icon(ISMSnoopInstrument * ism) const
{
	Instrument::Icon result;

	int width, height, depth;

	ismsnoop_->get_panel_icon_size(ism, &width, &height, &depth);

	if (width > 0 && height > 0 && depth > 0)
	{
		result.width = width;
		result.height = height;

		result.bytes.resize(width * height * (depth / 8));

		ismsnoop_->get_panel_icon_bytes(ism, result.bytes.data());
	}

	return result;
}
