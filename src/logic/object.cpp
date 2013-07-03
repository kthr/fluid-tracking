/*
 * object.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "object.hpp"

namespace elib
{

Object::Object()
{
}

Object::~Object()
{
	// TODO Auto-generated destructor stub
}

uint32_t Object::getFrameId() const
{
	return frameId;
}

void Object::setFrameId(uint32_t frameId)
{
	this->frameId = frameId;
}

uint32_t Object::getId() const
{
	return id;
}

void Object::setId(uint32_t id)
{
	this->id = id;
}

const Mask2D* Object::getMask() const
{
	return mask;
}

void Object::setMask(const Mask2D* mask)
{
	this->mask = mask;
}

uint32_t Object::getTrackId() const
{
	return trackId;
}

void Object::setTrackId(uint32_t trackId)
{
	this->trackId = trackId;
}
bool Object::isValid() const
{
	return valid;
}

void Object::setValid(bool valid)
{
	this->valid = valid;
}
void Object::toXML(const xmlTextWriterPtr writer, bool compressed) const
{
	int rc;
	std::stringstream tmp;

	rc = xmlTextWriterStartElement(writer, BAD_CAST "centroid"); /* start centroid */
	rc = xmlTextWriterStartElement(writer, BAD_CAST "p"); /* start point */
	tmp << "";
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "x", BAD_CAST tmp.str().c_str()); /*  x */
	tmp << "";
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "y", BAD_CAST tmp.str().c_str()); /*  y */
	rc = xmlTextWriterEndElement(writer); /* end p */
	rc = xmlTextWriterEndElement(writer); /* end centroid */
	if(compressed)
	{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "mask"); /* start centroid */
		rc = xmlTextWriterEndElement(writer);
	}
	else
	{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "outline"); /* start centroid */
		rc = xmlTextWriterEndElement(writer);
	}
}

elib::Annotation* Object::addAnnotation()
{
	annotations.push_back(Annotation());
	return &(annotations.back());
}

elib::Link* Object::addLink()
{
	links.push_back(Link());
	return &(links.back());
}

} /* namespace elib */

