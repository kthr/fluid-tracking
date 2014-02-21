/*
 * object.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "object.hpp"

#include <sstream>

#include "io/xmlExport.hpp"
#include "templates/boundingBox.hpp"
#include "trackingData.hpp"
#include "utils/rle.hpp"

namespace elib
{

Object::Object()
{
}

Object::~Object()
{
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

const std::shared_ptr<Mask<glm::ivec2>> Object::getMask() const
{
	return mask;
}

void Object::setMask(std::shared_ptr<Mask<glm::ivec2>> &mask)
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
	mask->getOutline(outline, centroid);

	xmlTextWriterStartElement(writer, BAD_CAST "centroid"); /* start centroid */
	writePoint(writer, centroid);
	xmlTextWriterEndElement(writer); /* end centroid */
	if(compressed)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "mask"); /* start mask */
		xmlTextWriterStartElement(writer, BAD_CAST "bbox"); /* start bbox */
		BoundingBox<glm::ivec2> bbox = mask->getBoundingBox();
		writePoint(writer, bbox.getUpperLeft());
		writePoint(writer, bbox.getBottomRight());
		xmlTextWriterEndElement(writer); /* end mask */
		xmlTextWriterWriteElement(writer, BAD_CAST "rleString", BAD_CAST RLE::binary_encode(mask->getBoxMask()).c_str()); /* start centroid */
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

	xmlTextWriterStartElement(writer, BAD_CAST "point"); /* start point */
	tmp << point.x;
	XMLExport::writeElement(writer, "x", tmp.str()); /*  x */
	tmp.str("");
	tmp << point.y;
	XMLExport::writeElement(writer, "y", tmp.str()); /*  y */
	xmlTextWriterEndElement(writer); /* end point */
}
} /* namespace elib */

