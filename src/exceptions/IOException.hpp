/*
 * genericException.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#ifndef IOEXCEPTION_CPP_
#define IOEXCEPTION_CPP_

#include <exception>
#include <string>

namespace elib{

class IOException : virtual public std::exception {

	public:
		IOException(const char* message)
		: message(std::string(message))
		{
		}
		IOException(std::string message)
		: message(message)
		{
		}
		virtual const char* what() const throw() override
		{
			return message.c_str();
		}
	private:
		std::string message;
};

}

#endif /* IOEXCEPTION_CPP_ */