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

#include "frame.hpp"

namespace elib
{

class TrackingData
{
	public:
		TrackingData();
		TrackingData(std::vector<MaskList2D> *frames, bool compressed=true);
		virtual ~TrackingData();

		Frame* addFrame();
		uint32_t getNumFrames() const;
		uint32_t getNumTracks() const;
		void construct();
		void toXML(const xmlTextWriterPtr writer) const;

	private:
		std::vector<MaskList2D> *data;
		std::vector<Frame> frames;
		std::set<uint32_t> tracks;
		bool compressed;
};

} /* namespace elib */
#endif /* TRACKINGDATA_HPP_ */
