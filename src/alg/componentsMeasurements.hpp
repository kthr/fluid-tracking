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

		ComponentsMeasurements() noexcept;
		ComponentsMeasurements(const ComponentsMeasurements& other);
		ComponentsMeasurements(ComponentsMeasurements&& other);
		ComponentsMeasurements& operator=(const ComponentsMeasurements &other);
		ComponentsMeasurements& operator=(ComponentsMeasurements &&other);
		ComponentsMeasurements(Image<int> &image);
		virtual ~ComponentsMeasurements();
		elib::MaskList<int, glm::ivec2> getMasks();
	private:

		short connectivity = LARGE_CONNECTIVITY;
		Image<int> label_image;
		std::set<int> labels;
		MaskList<int, glm::ivec2> masks;
		int num_labels = 0;

		void init();
};

} /* namespace elib */

#endif /* COMPONENTSMEASUREMENTS_HPP_ */
