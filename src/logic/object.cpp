/*
 * object.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "object.hpp"

#include <sstream>

#include "io/xmlExport.hpp"
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

int Object::getFrameId() const
{
	return frameId;
}

void Object::setFrameId(int frameId)
{
	this->frameId = frameId;
}

int Object::getId() const
{
	return id;
}

void Object::setId(int id)
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

int Object::getTrackId() const
{
	return trackId;
}

void Object::setTrackId(int trackId)
{
	this->trackId = trackId;
}
void Object::toXML(const xmlTextWriterPtr writer, bool compressed) const
{
	std::vector<glm::ivec2> outline;
	glm::ivec2 centroid(0,0);
	const_cast<Mask2D*>(mask)->getOutline(outline, centroid);

	xmlTextWriterStartElement(writer, BAD_CAST "centroid"); /* start centroid */
	writePoint(writer, centroid);
	xmlTextWriterEndElement(writer); /* end centroid */
	if(compressed)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "mask"); /* start mask */
		xmlTextWriterStartElement(writer, BAD_CAST "bbox"); /* start bbox */
		std::vector<glm::ivec2> bbox = const_cast<Mask2D*>(mask)->getBoundingBox();
		for(unsigned int i=0; i<bbox.size(); ++i)
		{
			writePoint(writer, bbox[i]);
		}
		xmlTextWriterEndElement(writer); /* end mask */
		xmlTextWriterWriteElement(writer, BAD_CAST "d", BAD_CAST RLE::binary_encode(const_cast<Mask2D*>(mask)->getBoxMask()).c_str()); /* start centroid */
		xmlTextWriterEndElement(writer); /* end mask */
	}
	else
	{
		xmlTextWriterStartElement(writer, BAD_CAST "outline"); /* start outline */
		for(unsigned int i=0; i<outline.size();++i)
		{
			writePoint(writer, outline[i]);
		}
		xmlTextWriterEndElement(writer); /* end outline */
	}

	xmlTextWriterStartElement(writer, BAD_CAST "links"); /* start links */
	for(unsigned int i=0; i<links.size(); ++i)
	{
		links[i].toXML(writer);
	}
	xmlTextWriterEndElement(writer); /* end links */
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
	std::stringstream tmp;

	xmlTextWriterStartElement(writer, BAD_CAST "p"); /* start point */
	tmp << point.x;
	xmlTextWriterWriteAttribute(writer, BAD_CAST "x", BAD_CAST tmp.str().c_str()); /*  x */
	tmp.str("");
	tmp << point.y;
	xmlTextWriterWriteAttribute(writer, BAD_CAST "y", BAD_CAST tmp.str().c_str()); /*  y */
	xmlTextWriterEndElement(writer); /* end point */
}
} /* namespace elib */

