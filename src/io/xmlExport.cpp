/*
 * xmlExport.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#include "xmlExport.hpp"

#include <string>
#include <sstream>
#include <time.h>

#define ELIB_XML_ENCODING "ISO-8859-1"

namespace elib
{

XMLExport::XMLExport()
{
	// TODO Auto-generated constructor stub

}
XMLExport::~XMLExport()
{
	// TODO Auto-generated destructor stub
}

void XMLExport::write(const char *uri)
{
	int rc;
	xmlTextWriterPtr writer;
	std::stringstream tmp;

	writer = xmlNewTextWriterFilename(uri, 0);
	if (writer == NULL)
	{
		//throw
		return;
	}
	xmlTextWriterSetIndent(writer, 1);
	/* start document */
	rc = xmlTextWriterStartDocument(writer, NULL, ELIB_XML_ENCODING, NULL);

		/* start data-frame */
		rc = xmlTextWriterStartElement(writer, BAD_CAST "data-frame");
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version", BAD_CAST "1.0");
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "compressed", BAD_CAST "true");

			/* start description */
			rc = xmlTextWriterStartElement(writer, BAD_CAST "description");
				time_t rawtime; /* start date */
				struct tm * ptm;
				time(&rawtime);
				ptm = gmtime(&rawtime);
				tmp << ptm->tm_year + 1900 << "-" << ptm->tm_mon +1 << "-" << ptm->tm_mday << " " << ptm->tm_hour << ":" << ptm->tm_min << ":" << ptm->tm_sec;
				xmlTextWriterWriteElement(writer, BAD_CAST "date", BAD_CAST tmp.str().c_str()); /* end date */
				xmlTextWriterWriteElement(writer, BAD_CAST "number_of_frames", BAD_CAST ""); /* end number of frames */
				xmlTextWriterWriteElement(writer, BAD_CAST "number_of_tracks", BAD_CAST ""); /* end number of tracks */
				xmlTextWriterWriteElement(writer, BAD_CAST "total_number_of_objects", BAD_CAST ""); /* end total number of objects */
				rc = xmlTextWriterStartElement(writer, BAD_CAST "tracking_parameter"); /* start tracking parameter */
				rc = xmlTextWriterStartElement(writer, BAD_CAST "param");
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST "C0");
					rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "value", BAD_CAST ".8");
				rc = xmlTextWriterEndElement(writer); /* end param */
				rc = xmlTextWriterEndElement(writer); /* end tracking parameter */
				xmlTextWriterWriteElement(writer, BAD_CAST "details ", BAD_CAST ""); /* end total number of objects */
			/* end description */
			rc = xmlTextWriterEndElement(writer);

		/* end data-frame */
		rc = xmlTextWriterEndElement(writer);
	/* end document */
	rc = xmlTextWriterEndDocument(writer);

	xmlFreeTextWriter(writer);
}
} /* namespace elib */
