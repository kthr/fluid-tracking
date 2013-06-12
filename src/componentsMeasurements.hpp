/*
 * componentsMeasurements.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#ifndef COMPONENTSMEASUREMENTS_HPP_
#define COMPONENTSMEASUREMENTS_HPP_

#include <stdint.h>
#include <set>
#include <vector>
#include <tr1/unordered_map>

#include "c_wrapper.h"
#include "mask.hpp"

class ComponentsMeasurements
{
	public:
		ComponentsMeasurements(cimage *label_image);
		virtual ~ComponentsMeasurements();

		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;
		static short connectivity;

	private:
		void init();

		cimage * label_image;
		int32_t num_labels;
		std::set<int32_t> labels;
		std::tr1::unordered_map<int32_t,int32_t> labels2masks_map;
		std::vector<Mask<glm::ivec3> > masks;
};

#endif /* COMPONENTSMEASUREMENTS_HPP_ */
