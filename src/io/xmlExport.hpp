/*
 * xmlExport.hpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#ifndef XMLEXPORT_HPP_
#define XMLEXPORT_HPP_

#include <string>

#include "utils/parameters.hpp"
#include "logic/trackingData.hpp"

namespace elib
{

class XMLExport
{
	public:
		XMLExport(const Parameters *params, const TrackingData *data);
		virtual ~XMLExport();

		void write(const char *uri);
		template <typename T>
		static int writeElement(xmlTextWriterPtr writer, std::string name, T value)
		{
			std::stringstream tmp;
			tmp << value;
			return xmlTextWriterWriteElement(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
		}
		template <typename T>
		static int writeAttribute(xmlTextWriterPtr writer, std::string name, T value)
		{
			std::stringstream tmp;
			tmp << value;
			return xmlTextWriterWriteAttribute(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
		}
	private:
		const Parameters *params;
		const TrackingData *data;
};

} /* namespace elib */
#endif /* XMLEXPORT_HPP_ */
