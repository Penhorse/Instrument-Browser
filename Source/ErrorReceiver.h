#ifndef ERRORRECEIVER_H_INCLUDED
#define ERRORRECEIVER_H_INCLUDED

#include <string>

class ErrorReceiver
{

public:

	virtual void receive_error(const std::string & what) = 0;
	virtual void clear_errors() = 0;
};

#endif  // ERRORRECEIVER_H_INCLUDED
