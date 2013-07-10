/*
 * fluidTracks.hpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#ifndef FLUIDTRACKS_HPP_
#define FLUIDTRACKS_HPP_

#include <stdint.h>
#include <string>
#include <vector>

#include "templates/image.hpp"
#include "types.hpp"
#include "utils/parameters.hpp"
#include "utils/vectorComparators.hpp"

namespace elib
{

/* bad practice?? */
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
		FluidTracks(Parameters *params, vector<string> *images, vector<string> *flows);
		virtual ~FluidTracks();
		void addAppearingObjects(MaskList2D *masks);
		void applySizeConstraints(MaskList2D *masks);
		void detectDivisions(MaskList2D *masks);
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
		vector<MaskList2D >* getFrames();
		uint32_t getMaxObjectSize() const;
		void setMaxObjectSize(uint32_t maxObjectSize = UINT32_MAX);

	private:
		bool include_appearing = true;
		Image<int32_t> initial;
		vector<glm::ivec2> *divisions;
		Parameters *params;
		string initial_mask_image = "";
		uint32_t id_counter = 0;
		uint32_t min_object_size=0,
				 max_object_size=UINT32_MAX;
		vector<string> 	*images=nullptr,
						*flows=nullptr;
		vector<MaskList2D> *frames;
};

} /* namespace elib */
#endif /* FLUIDTRACKS_HPP_ */
