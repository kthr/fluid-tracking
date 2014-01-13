/*
 * fluidTracks.hpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#ifndef FLUIDTRACKS_HPP_
#define FLUIDTRACKS_HPP_

#include <limits>
#include <string>
#include <vector>

#include "templates/image.hpp"
#include "templates/maskList.hpp"
#include "utils/parameters.hpp"

namespace elib
{

class FluidTracks
{
	public:
		FluidTracks();
		FluidTracks(Parameters *params, std::vector<std::string> *images, std::vector<std::string> *flows);
		virtual ~FluidTracks();
		void addAppearingObjects(MaskList<int, glm::ivec2> &masks);
		void applySizeConstraints(MaskList<int, glm::ivec2> &masks);
		void detectDivisions(MaskList<int, glm::ivec2> &masks);
		void track();
		bool isIncludeAppearing() const
		{
			return include_appearing;
		}
		void setIncludeAppearing(bool includeAppearing = true)
		{
			include_appearing = includeAppearing;
		}
		int getMinObjectSize() const
		{
			return min_object_size;
		}
		void setMinObjectSize(int minObjectSize = 0)
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
		Image<int>* getInitial();
		std::vector<MaskList<int, glm::ivec2> >* getFrames();
		int getMaxObjectSize() const;
		void setMaxObjectSize(int maxObjectSize = std::numeric_limits<int32_t>::max());
		std::string getImage(unsigned int i)
		{
			if(images->size() > i)
				return (*images)[i];
			else
				return std::string("");
		}
		std::string getFlow(unsigned int i)
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
		Image<int> initial;
		std::vector<glm::ivec2> *divisions;
		Parameters *params;
		std::string initial_mask_image = "";
		int id_counter = 0;
		int min_object_size=0,
			max_object_size=std::numeric_limits<int32_t>::max();
		std::vector<std::string> *images=nullptr,
								 *flows=nullptr;
		std::vector<MaskList<int, glm::ivec2>> *frames;
		int verbosity = 0,
			cycles = -1;
};

} /* namespace elib */
#endif /* FLUIDTRACKS_HPP_ */
