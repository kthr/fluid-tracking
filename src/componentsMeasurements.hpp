/*
 * componentsMeasurements.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#ifndef COMPONENTSMEASUREMENTS_HPP_
#define COMPONENTSMEASUREMENTS_HPP_

#include <set>
#include <stdint.h>
#include <tr1/unordered_map>
#include <vector>

#include "../lib/glm/glm.hpp"
#include "c_wrapper.h"
#include "connectedComponents.hpp"
#include "templates/mask.hpp"

namespace elib {

class ComponentsMeasurements
{
	public:
		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;
		static short connectivity;

		ComponentsMeasurements(cimage *label_image);
		virtual ~ComponentsMeasurements();
		bool deleteMask(int32_t label);
		Mask<glm::vec3>* getMask(int32_t label);
		cimage* masksToImage(uint32_t rank, uint32_t *dimensions);

	private:
		void init();

		cimage * label_image;
		int32_t num_labels;
		std::set<int32_t> labels;
		std::tr1::unordered_map<int32_t,Mask<glm::vec3> > labels2masks_map;
};

} /* namespace elib */

#endif /* COMPONENTSMEASUREMENTS_HPP_ */
