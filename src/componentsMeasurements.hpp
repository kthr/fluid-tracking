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

#include "lib/eigen/SparseCore"

class ComponentsMeasurements
{
	public:
		ComponentsMeasurements(cimage *label_image);
		virtual ~ComponentsMeasurements();

	private:
		void init();

		cimage * label_image;
		bool binary = false;
		int32_t num_labels = 0;
		std::set<int32_t> labels;
		std::tr1::unordered_map<int32_t,int32_t> labels2masks_map;
		std::vector<Eigen::SparseVector<int32_t> > masks;
};

#endif /* COMPONENTSMEASUREMENTS_HPP_ */
