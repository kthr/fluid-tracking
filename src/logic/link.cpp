/*
 * link.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "link.hpp"

#include <sstream>

#include "io/xmlExport.hpp"
#include "trackingData.hpp"

namespace elib
{

Link::Link()
{
	// TODO Auto-generated constructor stub

}

Link::~Link()
{
	// TODO Auto-generated destructor stub
}

const Object*& Link::getFrom()
{
	return from;
}

void Link::setFrom(const Object* from)
{
	this->from = from;
}
void Link::setTo(const Object* to)
{
	this->to.push_back(to);
}

int Link::getType() const
{
	return type;
}

void Link::toXML(const xmlTextWriterPtr writer) const
{
	std::string typeName;

	xmlTextWriterStartElement(writer, BAD_CAST "link"); /* start link */
	switch(type)
	{
		case SUCCESSOR:
			typeName = "$SUCCESSOR$";
			break;
		case DIVISION:
			typeName = "$DIVISION$";
			break;
		default:
			typeName = "$ERROR$";
			break;
	}
	XMLExport::writeAttribute(writer, "type", typeName);
	XMLExport::writeAttribute(writer, "probability", probability);
	XMLExport::writeAttribute(writer, "v", TrackingData::DEFAULT_VALIDITY);
	for(unsigned int i=0; i<to.size(); ++i)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "object"); /* start object */
		XMLExport::writeAttribute(writer, "objectID", to[i]->getId());
		XMLExport::writeAttribute(writer, "frameID", to[i]->getFrameId());
		XMLExport::writeAttribute(writer, "v", TrackingData::DEFAULT_VALIDITY);
		xmlTextWriterEndElement(writer); /* end object */
	}
	xmlTextWriterEndElement(writer); /* end link */
}

void Link::setType(int type)
{
	this->type = type;
}

} /* namespace elib */
