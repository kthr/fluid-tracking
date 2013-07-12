/*
 * object.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "object.hpp"

#include <sstream>

#include "trackingData.hpp"
#include "utils/rle.hpp"

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
void Object::toXML(const xmlTextWriterPtr writer, bool compressed) const
{
	int rc;
	std::stringstream tmp;

	std::vector<glm::ivec2> outline;
	glm::ivec2 centroid(0,0);
	const_cast<Mask2D*>(mask)->getOutline(outline, centroid);

	rc = xmlTextWriterStartElement(writer, BAD_CAST "centroid"); /* start centroid */
	writePoint(writer, centroid);
	rc = xmlTextWriterEndElement(writer); /* end centroid */
	if(compressed)
	{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "mask"); /* start mask */
		rc = xmlTextWriterStartElement(writer, BAD_CAST "bbox"); /* start bbox */
		std::vector<glm::ivec2> bbox = const_cast<Mask2D*>(mask)->getBoundingBox();
		for(int i=0; i<bbox.size(); ++i)
		{
			writePoint(writer, bbox[i]);
		}
		rc = xmlTextWriterEndElement(writer); /* end mask */
		rc = xmlTextWriterWriteElement(writer, BAD_CAST "d", BAD_CAST RLE::binary_encode(const_cast<Mask2D*>(mask)->getBoxMask()).c_str()); /* start centroid */
		rc = xmlTextWriterEndElement(writer); /* end mask */
	}
	else
	{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "outline"); /* start outline */
		for(int i=0; i<outline.size();++i)
		{
			writePoint(writer, outline[i]);
		}
		rc = xmlTextWriterEndElement(writer); /* end outline */
	}

	rc = xmlTextWriterStartElement(writer, BAD_CAST "links"); /* start links */
	for(int i=0; i<links.size(); ++i)
	{
		links[i].toXML(writer);
	}
	rc = xmlTextWriterEndElement(writer); /* end links */
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

void Object::writePoint(const xmlTextWriterPtr writer, const glm::ivec2 &point) const
{
	int rc;
	std::stringstream tmp;

	rc = xmlTextWriterStartElement(writer, BAD_CAST "p"); /* start point */
	tmp << point.x;
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "x", BAD_CAST tmp.str().c_str()); /*  x */
	tmp.str("");
	tmp << point.y;
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "y", BAD_CAST tmp.str().c_str()); /*  y */
	rc = xmlTextWriterEndElement(writer); /* end point */
}
} /* namespace elib */

