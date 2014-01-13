/*
 * trackingData.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "trackingData.hpp"

#include <boost/filesystem.hpp>
#include <sstream>

#include "io/xmlExport.hpp"
#include "object.hpp"

namespace elib
{

TrackingData::TrackingData()
{
	data = nullptr;
}
TrackingData::TrackingData(elib::FluidTracks *ft, bool compressed)
: ft(ft), data(ft->getFrames()), compressed(compressed)
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
int TrackingData::getNumFrames() const
{
	return frames.size();
}
int TrackingData::getNumTracks() const
{
	return tracks.size();
}
int TrackingData::getNumberObjects() const
{
	int count = 0;

	for(unsigned int i=0; i<data->size(); ++i)
	{
		count += (*data)[i].getSize();
	}
	return count;
}
void TrackingData::construct()
{
	if (data != nullptr)
	{
		namespace fs = boost::filesystem;
		std::string path = ft->getImage(0);
		if(path.compare("") != 0)
		{
			image_path = fs::canonical(fs::complete(fs::path(path).remove_filename())).string();
		}
		path = ft->getFlow(0);
		if(path.compare("") != 0)
		{
			flow_path = fs::canonical(fs::complete(fs::path(path).remove_filename())).string();
		}
		constructDivisions();

		Object *object;
		Link *link;
		int id = 0, trackId = 0;
		unsigned int frameId = 0;
		std::vector<MaskList<int, glm::ivec2>>::iterator it;
		std::vector<Object>::iterator objectIt;

		for (it = data->begin(); it != data->end(); ++it)
		{
			id=0;
			Frame *frame = addFrame();
			for (auto mask = it->begin(); mask != it->end(); ++mask)
			{
				tracks.insert(mask->first); // insert track id
				object = frame->addObject(mask->first);
				object->setId(id);
				object->setFrameId(frameId);
				object->setMask(mask->second);
				++id;
			}
			++frameId;
		}
		for (frameId = 0; frameId < data->size()-1; ++frameId)
		{
			for (objectIt = frames[frameId].begin(); objectIt != frames[frameId].end(); ++objectIt)
			{
				trackId = objectIt->getTrackId();
				if (((*data)[frameId+1]).getMask(trackId) != nullptr)
				{
					link = objectIt->addLink();
					link->setType(Link::SUCCESSOR);
					link->setFrom(&*objectIt);
					link->setTo(frames[frameId+1].getObject(trackId));
				}
				else
				{
					std::unordered_map<int, std::set<int> >::iterator it;
					it = divisions.find(trackId);
					if(it != divisions.end())
					{
						link = objectIt->addLink();
						link->setType(Link::DIVISION);
						link->setFrom(&*objectIt);
						std::set<int>::iterator sit;
						for(sit=it->second.begin(); sit!=it->second.end(); ++sit)
						{
							link->setTo(frames[frameId+1].getObject(*sit));
						}
					}
				}
			}
		}
	}
}
void TrackingData::constructDivisions()
{
	const std::vector<glm::ivec2> div = *(ft->getDivisions());
	std::unordered_map<int, std::set<int> >::iterator it;

	for(unsigned int i=0; i<div.size(); ++i)
	{
		it = divisions.find((div[i].x));
		if(it == divisions.end())
		{
			divisions.insert(std::pair<int,std::set<int> >(div[i].x,std::set<int>()));
			it = divisions.find((div[i].x));
			it->second.insert(div[i].y);
		}
		else
		{
			it->second.insert(div[i].y);
		}
	}
}
void TrackingData::toXML(const xmlTextWriterPtr writer) const
{
	xmlTextWriterStartElement(writer, BAD_CAST "frames"); /* start frames */
	XMLExport::writeAttribute(writer, "number_of_frames", getNumFrames());
	for (unsigned int i = 0; i < frames.size(); ++i)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "frame"); /* start frame */
		XMLExport::writeAttribute(writer, "id", i);
		XMLExport::writeAttribute(writer, "number_of_objects", frames[i].getNumObjects());
		XMLExport::writeAttribute(writer, "v", DEFAULT_VALIDITY);
		frames[i].toXML(writer, compressed);
		xmlTextWriterEndElement(writer); /* end frame */
	}
	xmlTextWriterEndElement(writer); /* end frames */

}

} /* namespace elib */
