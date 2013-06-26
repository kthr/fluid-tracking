/*
 * xmlExport.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#include "xmlExport.hpp"

#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <string>
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
	xmlChar *tmp = nullptr;

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
				time_t rawtime;
				struct tm * ptm;
				time(&rawtime);
				ptm = gmtime(&rawtime);
				rc = xmlTextWriterStartElement(writer, BAD_CAST "date"); /* start date */
//				tmp = ConvertInput("2013", ELIB_XML_ENCODING);
				xmlTextWriterWriteElement(writer, BAD_CAST "date", BAD_CAST "2013");
				rc = xmlTextWriterEndElement(writer); /* end date */
				//if(tmp != nullptr) xmlFree(tmp);
			/* end description */
			rc = xmlTextWriterEndElement(writer);

		/* end data-frame */
		rc = xmlTextWriterEndElement(writer);
	/* end document */
	rc = xmlTextWriterEndDocument(writer);

	xmlFreeTextWriter(writer);
}

} /* namespace elib */
