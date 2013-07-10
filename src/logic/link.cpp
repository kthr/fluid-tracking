/*
 * link.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "link.hpp"

#include <sstream>

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

const Object*& Link::getTo()
{
	return to;
}

void Link::setTo(const Object* to)
{
	this->to = to;
}

uint32_t Link::getType() const
{
	return type;
}

void Link::toXML(const xmlTextWriterPtr writer) const
{
	int rc;
	std::stringstream tmp;

	rc = xmlTextWriterStartElement(writer, BAD_CAST "link"); /* start centroid */
	switch(type)
	{
		case PREDECCESSOR:
			tmp << "$PREDECCESSOR$";
			break;
		case SUCCESSOR:
			tmp << "$SUCCESSOR$";
			break;
		case DIVISION:
			tmp << "$DIVISION$";
			break;
	}
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "type", BAD_CAST tmp.str().c_str());
	tmp.str("");
	tmp << 1.;
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "probability", BAD_CAST tmp.str().c_str());
	tmp.str("");
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "v", BAD_CAST "1");
	rc = xmlTextWriterStartElement(writer, BAD_CAST "object"); /* start object */
	tmp << to->getId();
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "objectID", BAD_CAST tmp.str().c_str());
	tmp.str("");
	tmp << to->getFrameId();
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "frameID", BAD_CAST tmp.str().c_str());
	tmp.str("");
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "v", BAD_CAST "1");
	rc = xmlTextWriterEndElement(writer); /* end object */
	rc = xmlTextWriterEndElement(writer);
}

void Link::setType(uint32_t type)
{
	this->type = type;
}

} /* namespace elib */
