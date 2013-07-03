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
#include <time.h>

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
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "compressed", BAD_CAST "1");

	/*************************************
	 * start description
	 *************************************/
	rc = xmlTextWriterStartElement(writer, BAD_CAST "description");
	xmlTextWriterWriteElement(writer, BAD_CAST "date", BAD_CAST getTime().c_str()); /* date */
	xmlTextWriterWriteElement(writer, BAD_CAST "number_of_tracks", BAD_CAST ""); /* number of tracks */
	xmlTextWriterWriteElement(writer, BAD_CAST "total_number_of_objects", BAD_CAST ""); /* total number of objects */
	params->toXML(writer); /* tracking parameters */
	xmlTextWriterWriteElement(writer, BAD_CAST "details ", BAD_CAST ""); /* total number of objects */

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

std::string XMLExport::getTime()
{
	std::stringstream tmp;
	time_t rawtime;
	struct tm * ptm;

	time(&rawtime);
	ptm = localtime(&rawtime);
	tmp << ptm->tm_year + 1900 << "-" << ptm->tm_mon +1 << "-" << ptm->tm_mday << " " << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec;
	return tmp.str();
}
} /* namespace elib */
