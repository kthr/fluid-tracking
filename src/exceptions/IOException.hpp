/*
 * genericException.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#ifndef IOEXCEPTION_CPP_
#define IOEXCEPTION_CPP_

#include <exception>

namespace elib{

class IOException : virtual public std::exception {

	public:
		virtual const char* what() const throw() override
		{
			return "IO error, image could not be loaded!";
		}
} IOException;

}

#endif /* IOEXCEPTION_CPP_ */
