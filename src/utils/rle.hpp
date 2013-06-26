/*
 * RLE.hpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#ifndef RLE_HPP_
#define RLE_HPP_

#include <string>

namespace elib
{

class RLE
{
	public:
		RLE();
		virtual ~RLE();
		static std::string encode( const std::string & to_encode );
		static std::string decode ( const std::string & to_decode );
};

} /* namespace elib */
#endif /* RLE_HPP_ */
