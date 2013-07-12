/*
 * RLE.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#include "rle.hpp"

#include <iostream>
#include <sstream>
#include <boost/regex.hpp>
#include <cstdlib>

namespace elib
{

RLE::RLE()
{
	// TODO Auto-generated constructor stub

}

RLE::~RLE()
{
	// TODO Auto-generated destructor stub
}


std::string RLE::binary_encode( const std::string & to_encode )
{
	char current, code;
	int count;
	std::string::const_iterator it;
	std::stringstream encoded;

	it = to_encode.begin();
	while(it!=to_encode.end())
	{
		current = *it;
		count = 1;
		++it;
		while(it!=to_encode.end() && current == *it)
		{
			++count;
			++it;
		}
		if(current == '0')
			code = 'B';
		else
			code = 'W';
		encoded << count << code;
	}
	return encoded.str();
}

std::string RLE::binary_decode( const std::string & to_decode )
{
   boost::regex e ( "(\\d+)(\\w)" ) ;
   boost::match_results<std::string::const_iterator> matches ;
   std::ostringstream oss ;
   std::string::const_iterator start = to_decode.begin( ) , end = to_decode.end( ) ;
   while ( boost::regex_search ( start , end , matches , e ) ) {
      std::string numberstring ( matches[ 1 ].first , matches[ 1 ].second ) ;
      int number = atoi( numberstring.c_str( ) ) ;
      std::string character ( matches[ 2 ].first , matches[ 2 ].second ) ;
      for ( int i = 0 ; i < number ; i++ )
	 oss << character ;
      start = matches[ 2 ].second ;
   }
   return oss.str( ) ;
}

} /* namespace elib */
