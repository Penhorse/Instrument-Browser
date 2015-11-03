/*
  ==============================================================================

    InstrumentReceiver.h
    Created: 31 Oct 2015 6:04:43pm
    Author:  Chris

  ==============================================================================
*/

#ifndef INSTRUMENTRECEIVER_H_INCLUDED
#define INSTRUMENTRECEIVER_H_INCLUDED

#include <string>

class Instrument;

class InstrumentReceiver
{

public:

	virtual void refresh_instruments() = 0;
	virtual void receive_instrument(const Instrument & instrument) = 0;
};

#endif  // INSTRUMENTRECEIVER_H_INCLUDED
