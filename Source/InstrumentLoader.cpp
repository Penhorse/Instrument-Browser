/*
  ==============================================================================

    InstrumentLoader.cpp
    Created: 31 Oct 2015 2:36:54pm
    Author:  Chris

  ==============================================================================
*/

#include "InstrumentLoader.h"

#include "Instrument.h"
#include "InstrumentReceiver.h"
#include "ISMSnoopWrapper.h"

InstrumentLoader::InstrumentLoader(const StringArray & directories, std::shared_ptr<ISMSnoopWrapper> ismsnoop, InstrumentReceiver * receiver) :
	Thread("InstrumentLoader"),
	directories_(directories),
	ismsnoop_(ismsnoop),
	receiver_(receiver)
{
	// nothing
}

void InstrumentLoader::run()
{
	for (const auto & dir : directories_)
	{
		if (File::isAbsolutePath(dir))
		{
			File dir_file(dir);

			if (dir_file.isDirectory())
			{
				DirectoryIterator directory_iterator(File(dir), true, "*.ism", File::TypesOfFileToFind::findFiles);

				while (directory_iterator.next())
				{
					if (threadShouldExit())
					{
						return;
					}

					File foundFile(directory_iterator.getFile());

					Instrument instrument;

					instrument.icon.width = 0;
					instrument.icon.height = 0;

					instrument.path = foundFile.getFullPathName().toStdString();

					const auto ism = ismsnoop_->open(instrument.path.c_str());

					if (ism)
					{
						int name_length = 0;

						ismsnoop_->get_name(ism, nullptr, &name_length);

						if (name_length > 0)
						{
							std::vector<char> buffer(name_length + 1);

							ismsnoop_->get_name(ism, buffer.data(), nullptr);

							instrument.name = std::string(buffer.begin(), buffer.end());
						}

						int width, height, depth;

						ismsnoop_->get_panel_icon_size(ism, &width, &height, &depth);

						if (width > 1 && height > 1 && depth > 1)
						{
							instrument.icon.width = width;
							instrument.icon.height = height;

							instrument.icon.bytes.resize(width * height * (depth / 8));

							ismsnoop_->get_panel_icon_bytes(ism, instrument.icon.bytes.data());
						}

						receiver_->receive_instrument(instrument);

						ismsnoop_->close(ism);
					}
				}
			}
		}
	}
}

