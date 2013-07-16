/*
 * link.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "link.hpp"

#include <sstream>

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

uint32_t Link::getType() const
{
	return type;
}

void Link::toXML(const xmlTextWriterPtr writer) const
{
	int rc;
	std::stringstream tmp;

	rc = xmlTextWriterStartElement(writer, BAD_CAST "link"); /* start link */
	switch(type)
	{
		case SUCCESSOR:
			tmp << "$SUCCESSOR$";
			break;
		case DIVISION:
			tmp << "$DIVISION$";
			break;
		default:
			tmp << "$ERROR$";
			break;
	}
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST tmp.str().c_str());
	tmp.str("");
	tmp << probability;
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "probability", BAD_CAST tmp.str().c_str());
	tmp.str("");
	tmp << TrackingData::DEFAULT_VALIDITY;
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "v", BAD_CAST tmp.str().c_str());
	tmp.str("");
	for(int i=0; i<to.size(); ++i)
	{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "object"); /* start object */
		tmp << to[i]->getId();
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "objectID", BAD_CAST tmp.str().c_str());
		tmp.str("");
		tmp << to[i]->getFrameId();
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "frameID", BAD_CAST tmp.str().c_str());
		tmp.str("");
		tmp << TrackingData::DEFAULT_VALIDITY;
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "v", BAD_CAST tmp.str().c_str());
		tmp.str("");
		rc = xmlTextWriterEndElement(writer); /* end object */
	}
	rc = xmlTextWriterEndElement(writer); /* end link */
}

void Link::setType(uint32_t type)
{
	this->type = type;
}

} /* namespace elib */
