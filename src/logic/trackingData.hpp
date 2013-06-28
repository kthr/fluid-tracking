/*
 * trackingData.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#ifndef TRACKINGDATA_HPP_
#define TRACKINGDATA_HPP_

#include <libxml/xmlwriter.h>
#include <set>
#include <stdint.h>
#include <types.hpp>
#include <vector>

namespace elib
{

class Frame;

class TrackingData
{
	public:
		TrackingData();
		explicit TrackingData(const std::vector<MaskList2D> *frames);
		virtual ~TrackingData();

		Frame* addFrame();
		uint32_t getNumFrames() const;
		uint32_t getNumTracks() const;
		void construct();
		void toXML(const xmlTextWriterPtr writer) const;

	private:
		const std::vector<MaskList2D> *data;
		std::vector<Frame> frames;
		std::set<uint32_t> tracks;
};

} /* namespace elib */
#endif /* TRACKINGDATA_HPP_ */
