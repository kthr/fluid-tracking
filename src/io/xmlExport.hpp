/*
 * xmlExport.hpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#ifndef XMLEXPORT_HPP_
#define XMLEXPORT_HPP_

#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>

namespace elib
{

class XMLExport
{
	public:
		XMLExport();
		virtual ~XMLExport();

		void write(const char *uri);
	private:
		xmlChar* convertInput(const char *in, const char *encoding);
};

} /* namespace elib */
#endif /* XMLEXPORT_HPP_ */
