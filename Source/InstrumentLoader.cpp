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

InstrumentLoader::InstrumentLoader(const StringArray & directories, InstrumentReceiver * receiver) :
	Thread("InstrumentLoader"),
	directories_(directories),
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

					receiver_->receive_instrument({ foundFile.getFullPathName() });
				}
			}
		}
	}
}

