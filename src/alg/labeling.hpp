/*
 * GraphCutImage.hpp
 *
 *  Created on: Jan 22, 2013
 *      Author: kthierbach
 */

#ifndef LABELING_HPP_
#define LABELING_HPP_

#include "templates/image.hpp"
#include "utils/parameters.hpp"

namespace elib{

class Labeling
{
	public:
		Labeling(){}
		~Labeling(){}
		std::shared_ptr<Image<int>> labeling(Image<int> &label_image, Image<int> &input_image, Parameters &input_params);

		private:
		int verbosity = 0,
			cycles = -1;
};

struct ForSmoothFn
{
		int *image;
		double lambda;
		int dummyLabel;
};

inline int label_dist(int value);
int smoothFn(int p1, int p2, int l1, int l2, void *data);

} /* namespace elib */

#endif /* LABELING_HPP_ */
