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

#include "glm/glm.hpp"
#include "templates/image.hpp"
#include "templates/maskList.hpp"

namespace elib {

class ComponentsMeasurements
{
	public:
		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;

		ComponentsMeasurements();
		ComponentsMeasurements(const ComponentsMeasurements& other);
		ComponentsMeasurements(ComponentsMeasurements&& other);
		ComponentsMeasurements(Image<int> &label_image);
		virtual ~ComponentsMeasurements();
		elib::MaskList<int, glm::ivec2> getMasks();
		ComponentsMeasurements& operator=(ComponentsMeasurements other)
		{
			swap(*this, other);
			return *this;
		}
	private:

		Image<int> label_image;
		int num_labels = 0;
		short connectivity = LARGE_CONNECTIVITY;
		std::set<int> *labels = nullptr;
		MaskList<int, glm::ivec2> *masks = nullptr;

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
