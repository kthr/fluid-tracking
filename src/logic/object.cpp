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

const Mask2D*& Object::getMask() const
{
	return mask;
}

void Object::setMask(const Mask2D*& mask)
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

} /* namespace elib */

void elib::Object::toXML(const xmlTextWriterPtr writer)
{
}
