/*
 * xmlExport.hpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#ifndef XMLEXPORT_HPP_
#define XMLEXPORT_HPP_

namespace elib
{

class XMLExport
{
	public:
		XMLExport();
		virtual ~XMLExport();

		void write(const char *uri);
};

} /* namespace elib */
#endif /* XMLEXPORT_HPP_ */
