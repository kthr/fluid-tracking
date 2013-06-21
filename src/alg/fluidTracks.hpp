/*
 * fluidTracks.hpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#ifndef FLUIDTRACKS_HPP_
#define FLUIDTRACKS_HPP_

#include <map>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "../templates/image.hpp"
#include "../templates/mask.hpp"
#include "../types.hpp"
#include "../utils/parameters.hpp"
#include "../utils/vectorComparators.hpp"
#include "componentsMeasurements.hpp"

namespace elib
{

using std::string;
using std::vector;
using std::map;
using std::unordered_map;
using elib::VectorComparators;
using elib::Mask;

class FluidTracks
{
	public:
		FluidTracks();
		FluidTracks(vector<string> *images, vector<string> *flows, Parameters *params);
		FluidTracks(vector<string> *images, Parameters *params);
		virtual ~FluidTracks();
		void addAppearingObjects(MaskList<int32_t, glm::ivec3> *masks);
		void applySizeConstraints(MaskList<int32_t, glm::ivec3> *masks);
		void detectDivisions(MaskList<int32_t, glm::ivec3> *masks);
		void track();
		bool isIncludeAppearing() const
		{
			return include_appearing;
		}
		void setIncludeAppearing(bool includeAppearing = true)
		{
			include_appearing = includeAppearing;
		}
		uint32_t getMinObjectSize() const
		{
			return min_object_size;
		}
		void setMinObjectSize(uint32_t minObjectSize = 0)
		{
			min_object_size = minObjectSize;
		}
		const string& getInitialMaskImage() const
		{
			return initial_mask_image;
		}
		void setInitialMaskImage(const string& initialMaskImage = NULL)
		{
			initial_mask_image = initialMaskImage;
		}
		Image<int32_t>* getInitial();
		vector<MaskList<int32_t, glm::ivec3> >*& getFrames();

	private:
		bool include_appearing = true;
		Image<int32_t> initial;
		vector<glm::ivec2> *divisions;
		Parameters *params;
		string initial_mask_image = "";
		uint32_t id_counter = 0;
		uint32_t min_object_size=0, max_object_size=UINT32_MAX;
		vector<string> 	*images=NULL, *flows=NULL;
		vector<MaskList<int32_t, glm::ivec3> > *frames;
};

} /* namespace elib */
#endif /* FLUIDTRACKS_HPP_ */
