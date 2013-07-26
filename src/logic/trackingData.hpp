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
#include <unordered_map>
#include <vector>

#include "alg/fluidTracks.hpp"
#include "frame.hpp"

namespace elib
{

class TrackingData
{
	public:
		TrackingData();
		TrackingData(elib::FluidTracks *frames, bool compressed=true);
		virtual ~TrackingData();

		Frame* addFrame();
		uint32_t getNumFrames() const;
		uint32_t getNumTracks() const;
		uint32_t getNumberObjects() const;
		void construct();
		void toXML(const xmlTextWriterPtr writer) const;

		const std::string& getFlowPath() const
		{
			return flow_path;
		}

		const std::string& getImagePath() const
		{
			return image_path;
		}

		bool isCompressed() const
		{
			return compressed;
		}

		void setCompressed(bool compressed)
		{
			this->compressed = compressed;
		}

		elib::FluidTracks* getFluidTracks() const
		{
			return ft;
		}

		const static int DEFAULT_VALIDITY = 0;

	private:
		elib::FluidTracks *ft;
		std::vector<MaskList2D> *data;
		std::vector<Frame> frames;
		std::set<int32_t> tracks;
		std::unordered_map<int32_t, std::set<int32_t> > divisions;
		bool compressed;
		std::string image_path = "",
					flow_path = "";

		void constructDivisions();
};

} /* namespace elib */
#endif /* TRACKINGDATA_HPP_ */
