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
Object* Frame::addObject(int trackId)
{
	objects.push_back(Object());
	track2objectId.insert(std::pair<int, int> (trackId, objects.size()-1));
	objects.back().setTrackId(trackId);
	return &objects.back();
}
std::vector<Object>::iterator Frame::end()
{
	return objects.end();
}
void Frame::toXML(const xmlTextWriterPtr writer, bool compressed) const
{
	for(unsigned int i=0; i<objects.size(); ++i)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "object"); /* start object */
		XMLExport::writeAttribute(writer, "id", i);
		XMLExport::writeAttribute(writer, "valid", TrackingData::DEFAULT_VALIDITY);
		objects[i].toXML(writer, compressed);
		xmlTextWriterEndElement(writer); /* end object */
	}
}
const Object* Frame::getObject(int trackId) const
{
	std::unordered_map<int, int>::const_iterator it = track2objectId.find(trackId);
	if(it != track2objectId.end())
		return &(objects[it->second]);
	else
		return nullptr;
}
int Frame::getNumObjects() const
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
