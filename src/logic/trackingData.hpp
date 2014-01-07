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
#include <unordered_map>
#include <vector>

#include "alg/fluidTracks.hpp"
#include "frame.hpp"
#include "templates/maskList.hpp"

namespace elib
{

class TrackingData
{
	public:
		TrackingData();
		TrackingData(elib::FluidTracks *frames, bool compressed=true);
		virtual ~TrackingData();

		Frame* addFrame();
		int getNumFrames() const;
		int getNumTracks() const;
		int getNumberObjects() const;
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
		std::vector<MaskList<int, glm::ivec2>> *data;
		std::vector<Frame> frames;
		std::set<int> tracks;
		std::unordered_map<int, std::set<int> > divisions;
		bool compressed;
		std::string image_path = "",
					flow_path = "";

		void constructDivisions();
};

} /* namespace elib */
#endif /* TRACKINGDATA_HPP_ */
