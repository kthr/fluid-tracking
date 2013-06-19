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
#include <unordered_map>
#include <vector>

#include "../lib/glm/glm.hpp"
#include "../templates/image.hpp"
#include "../templates/mask.hpp"
#include "../types.hpp"
#include "../utils/vectorComparators.hpp"
#include "connectedComponents.hpp"

namespace elib {

using std::unordered_map;
using elib::VectorComparators;


class ComponentsMeasurements
{
	public:
		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;

		ComponentsMeasurements();
		ComponentsMeasurements(Image<int32_t> *label_image);
		virtual ~ComponentsMeasurements();
		bool deleteMask(int32_t label);
		elib::mask2D* getMask(int32_t label);
		unordered_map<int32_t, mask2D > getMasks();
		Image<int32_t>* masksToImage(uint32_t rank, uint32_t *dimensions);
		const std::set<int32_t>& getLabels() const
		{
			return *labels;
		}
		int32_t getNumLabels() const
		{
			return num_labels;
		}
//		ComponentsMeasurements& operator=(ComponentsMeasurements other)
//		{
//			swap(*this, other);
//			return *this;
//		}
	private:

		Image<int32_t> *label_image = nullptr;
		int32_t num_labels = 0;
		short connectivity = SMALL_CONNECTIVITY;
		std::set<int32_t> *labels;
		unordered_map<int32_t, mask2D* > *masks;

		void init();
		friend void swap(ComponentsMeasurements& first, ComponentsMeasurements& second)
		{
			// enable ADL (not necessary in our case, but good practice)
			using std::swap;
			// by swapping the members of two classes,
			// the two classes are effectively swapped
			swap(first.connectivity, second.connectivity);
			swap(first.label_image, second.label_image);
			swap(first.labels, second.labels);
			swap(first.masks, second.masks);
			swap(first.num_labels, second.num_labels);
		}


};

} /* namespace elib */

#endif /* COMPONENTSMEASUREMENTS_HPP_ */
