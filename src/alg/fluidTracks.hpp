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

namespace elib
{

class FluidTracks
{
	public:
		FluidTracks();
		FluidTracks(Parameters *params, std::vector<std::string> *images, std::vector<std::string> *flows);
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
		const std::string& getInitialMaskImage() const
		{
			return initial_mask_image;
		}
		void setInitialMaskImage(const std::string& initialMaskImage = NULL)
		{
			initial_mask_image = initialMaskImage;
		}
		Image<int32_t>* getInitial();
		std::vector<MaskList2D >* getFrames();
		uint32_t getMaxObjectSize() const;
		void setMaxObjectSize(uint32_t maxObjectSize = UINT32_MAX);
		std::string getImage(int i)
		{
			if(images->size() > i)
				return (*images)[i];
			else
				return std::string("");
		}
		std::string getFlow(int i)
		{
			if(flows->size() > i)
				return (*flows)[i];
			else
				return std::string("");
		}
		const std::vector<glm::ivec2>* getDivisions() const
		{
			return divisions;
		}

		void setCycles(int cycles = -1)
		{
			this->cycles = cycles;
		}

		void setVerbosity(int verbosity = 0)
		{
			this->verbosity = verbosity;
		}

	private:
		bool include_appearing = true;
		Image<int32_t> initial;
		std::vector<glm::ivec2> *divisions;
		Parameters *params;
		std::string initial_mask_image = "";
		uint32_t id_counter = 0;
		uint32_t min_object_size=0,
				 max_object_size=UINT32_MAX;
		std::vector<std::string> 	*images=nullptr,
						*flows=nullptr;
		std::vector<MaskList2D> *frames;
		int verbosity = 0,
			cycles = -1;
};

} /* namespace elib */
#endif /* FLUIDTRACKS_HPP_ */
