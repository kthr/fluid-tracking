/*
 * frame.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "frame.hpp"

#include <sstream>

#include "io/xmlExport.hpp"
#include "trackingData.hpp"

namespace elib
{

Frame::Frame()
{
	// TODO Auto-generated constructor stub

}

Frame::~Frame()
{
	// TODO Auto-generated destructor stub
}

std::vector<Object>::iterator Frame::begin()
{
	return objects.begin();
}
Object* Frame::addObject(uint32_t trackId)
{
	objects.push_back(Object());
	track2objectId.insert(std::pair<uint32_t, uint32_t> (trackId, objects.size()-1));
	objects.back().setTrackId(trackId);
	return &objects.back();
}
std::vector<Object>::iterator Frame::end()
{
	return objects.end();
}
void Frame::toXML(const xmlTextWriterPtr writer, bool compressed) const
{
	for(uint32_t i=0; i<objects.size(); ++i)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "object"); /* start object */
		XMLExport::writeAttribute(writer, "id", i);
		XMLExport::writeAttribute(writer, "v", TrackingData::DEFAULT_VALIDITY);
		objects[i].toXML(writer, compressed);
		xmlTextWriterEndElement(writer); /* end object */
	}
}
const Object* Frame::getObject(uint32_t trackId) const
{
	std::unordered_map<uint32_t, uint32_t>::const_iterator it = track2objectId.find(trackId);
	if(it != track2objectId.end())
		return &(objects[it->second]);
	else
		return nullptr;
}
uint32_t Frame::getNumObjects() const
{
	return objects.size();
}

const std::vector<Object>& Frame::getObjects() const
{
	return objects;
}

void Frame::setObjects(const std::vector<Object>& objects)
{
	this->objects = objects;
}
} /* namespace elib */
