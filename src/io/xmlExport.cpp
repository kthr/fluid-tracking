/*
 * xmlExport.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#include "xmlExport.hpp"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <sstream>

#include "fluidTracking.hpp"
#include "utils/utilities.hpp"

#define ELIB_XML_ENCODING "ISO-8859-1"

namespace elib
{

XMLExport::XMLExport(const Parameters *params, const TrackingData *data)
: params(params), data(data)
{

}
XMLExport::~XMLExport()
{

}

void XMLExport::write(const char *uri)
{
	const xmlTextWriterPtr writer = xmlNewTextWriterFilename(uri, 0);;
	int rc;
	std::stringstream tmp;

	if (writer == NULL)
	{
		//throw
		return;
	}
	xmlTextWriterSetIndent(writer, 1);

	/*************************************
	 * start document
	 *************************************/
	rc = xmlTextWriterStartDocument(writer, NULL, ELIB_XML_ENCODING, NULL);

	/*************************************
	 * start data-frame
	 *************************************/
	rc = xmlTextWriterStartElement(writer, BAD_CAST "data-frame");
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST "1.0");
	if(data->isCompressed())
	{
		tmp << 1;
	}
	else
	{
		tmp << 0;
	}
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "compressed", BAD_CAST tmp.str().c_str());
	tmp.str("");

	/*************************************
	 * start description
	 *************************************/
	rc = xmlTextWriterStartElement(writer, BAD_CAST "description");
	xmlTextWriterWriteElement(writer, BAD_CAST "date", BAD_CAST Utilities::getTime().c_str()); /* date */
	tmp << data->getNumTracks();
	xmlTextWriterWriteElement(writer, BAD_CAST "number_of_tracks", BAD_CAST tmp.str().c_str()); /* number of tracks */
	tmp.str("");
	xmlTextWriterWriteElement(writer, BAD_CAST "total_number_of_objects", BAD_CAST ""); /* total number of objects */
	xmlTextWriterStartElement(writer, BAD_CAST "tracking_info"); /* start tracking info */
	tmp << REVISION;
	xmlTextWriterWriteElement(writer, BAD_CAST "revision", BAD_CAST tmp.str().c_str()); /* number of tracks */
	tmp.str("");
	tmp << data->getImagePath();
	xmlTextWriterWriteElement(writer, BAD_CAST "image-path", BAD_CAST tmp.str().c_str()); /* number of tracks */
	tmp.str("");
	tmp << data->getFlowPath();
	xmlTextWriterWriteElement(writer, BAD_CAST "flow-path", BAD_CAST tmp.str().c_str()); /* number of tracks */
	tmp.str("");
	params->toXML(writer); /* tracking parameters */
	rc = xmlTextWriterEndElement(writer); /* end tracking info */
	xmlTextWriterWriteElement(writer, BAD_CAST "details ", BAD_CAST ""); /* details */
	rc = xmlTextWriterEndElement(writer); /* end details */

	data->toXML(writer);

	/*************************************
	 * end description
	 *************************************/
	rc = xmlTextWriterEndElement(writer);

	/*************************************
	 * end data-frame
	 *************************************/
	rc = xmlTextWriterEndElement(writer);

	/*************************************
	 * end document
	 *************************************/
	rc = xmlTextWriterEndDocument(writer);

	xmlFreeTextWriter(writer);
}

} /* namespace elib */
