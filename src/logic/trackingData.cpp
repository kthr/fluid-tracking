/*
 * trackingData.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "trackingData.hpp"

#include <sstream>

#include "object.hpp"

namespace elib
{

TrackingData::TrackingData()
{
	data = nullptr;
}

TrackingData::TrackingData(const std::vector<MaskList2D> *frames) :
		data(frames)
{
}
TrackingData::~TrackingData()
{
	// TODO Auto-generated destructor stub
}

Frame* TrackingData::addFrame()
{
	frames.push_back(Frame());
	return &(frames.back());
}
uint32_t TrackingData::getNumFrames() const
{
	return frames.size();
}
uint32_t TrackingData::getNumTracks() const
{
	return tracks.size();
}
void TrackingData::construct()
{
	if (data != nullptr)
	{
		Frame *frame;
		Object *object;
		Link *link;
		uint32_t id = 0, frameId = 0;
		trackId = 0;
		std::vector<MaskList2D>::iterator it, next;
		std::vector<Frame>::iterator objectIt;
		std::unordered_map<uint32_t, Mask2D*>::iterator mask;

		for (it = data->begin(); it != data->end(); ++it)
		{
			Frame *frame = addFrame();
			for (mask = it->begin(); mask != it->end(); ++mask)
			{
				tracks.insert(mask->first);
				object = frame->addObject(mask->first);
				object.setId(id);
				object.setFrameId(frameId);
				object.setMask(it->second);
				++id;
			}
			++frameId;
		}
		for (uint32_t frameId = 0; frameId != data->size() - 1; ++frameId)
		{
			for (objectIt = frames[frameId].begin(); objectIt != frames[frameId].end(); ++objectIt)
			{
				trackId = objectIt->getTrackId();
				if (data[frameId + 1].getMask(trackId) != nullptr)
				{
					link = objectIt->addLink();
					ling->setType(Link::SUCCESSOR);
					link->setFrom(objectIt);
					link->setTo(frames[frameId + 1].getObject(trackId));
				}
			}
			++frameId;
		}
	}
}
void TrackingData::toXML(const xmlTextWriterPtr writer) const
{
	int rc;
	std::stringstream tmp;

	rc = xmlTextWriterStartElement(writer, BAD_CAST "frames"); /* start frames */
	tmp << getNumFrames();
	rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "number_of_frames", BAD_CAST tmp.str().c_str()); /*  number of frames */
	for (uint32_t i = 0; i < frames.size(); ++i)
	{
		rc = xmlTextWriterStartElement(writer, BAD_CAST "frame"); /* start frame */
		tmp << i;
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "id", BAD_CAST tmp.str().c_str()); /*  frame id */
		tmp.str("");
		tmp << frames[i].getNumObjects();
		rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "number_of_objects", BAD_CAST tmp.str().c_str()); /*  number of objects */
		frames[i].toXML(writer);
		rc = xmlTextWriterEndElement(writer); /* end frame */
	}
	rc = xmlTextWriterEndElement(writer); /* end frames */

}

} /* namespace elib */
