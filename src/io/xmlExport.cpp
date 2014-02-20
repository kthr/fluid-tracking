/*
 * xmlExport.cpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#include "xmlExport.hpp"

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

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

	if (writer == NULL)
	{
		//throw
		return;
	}
	xmlTextWriterSetIndent(writer, 1);

	/*************************************
	 * start document
	 *************************************/
	xmlTextWriterStartDocument(writer, NULL, ELIB_XML_ENCODING, NULL);

	/*************************************
	 * start data-frame
	 *************************************/
	xmlTextWriterStartElement(writer, BAD_CAST "celltrackingResults");
	writeAttribute(writer, "version", "1.0");
	writeAttribute(writer, "compressed", data->isCompressed());

	/*************************************
	 * start description
	 *************************************/
	xmlTextWriterStartElement(writer, BAD_CAST "description");
	writeElement(writer, "date", Utilities::getTime());
	if(data->getFluidTracks()->getInitial()->getRank() == 2)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "imageDimensions"); /* start image-dimensions */
		writeElement(writer, "width", data->getFluidTracks()->getInitial()->getWidth());
		writeElement(writer, "height", data->getFluidTracks()->getInitial()->getHeight());
		xmlTextWriterEndElement(writer); /* end image-dimensions */
	}
	writeElement(writer, "numberOfTracks", data->getNumTracks());
	writeElement(writer, "numberOfObjects", data->getNumberObjects());
	xmlTextWriterStartElement(writer, BAD_CAST "trackingInfo"); /* start tracking info */
	writeElement(writer, "revision", REVISION);
	writeElement(writer, "imagePath", data->getImagePath());
	writeElement(writer, "flowPath", data->getFlowPath());
	params->toXML(writer); /* tracking parameters */
	xmlTextWriterEndElement(writer); /* end tracking info */
	xmlTextWriterWriteElement(writer, BAD_CAST "details ", BAD_CAST ""); /* details */
	xmlTextWriterEndElement(writer); /* end details */

	data->toXML(writer);

	/*************************************
	 * end description
	 *************************************/
	xmlTextWriterEndElement(writer);

	/*************************************
	 * end data-frame
	 *************************************/
	xmlTextWriterEndElement(writer);

	/*************************************
	 * end document
	 *************************************/
	xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);
}

} /* namespace elib */
