/*
 * GraphCutImage.hpp
 *
 *  Created on: Jan 22, 2013
 *      Author: kthierbach
 */

#ifndef LABELING_HPP_
#define LABELING_HPP_

#include "c_wrapper.h"

namespace elib{

class Labeling
{
	public:
		Labeling();
		~Labeling();
		void labeling(cimage *new_label_image, cimage* label_image, cimage* input_image, parameters *input_params);
};

struct ForSmoothFn
		{
			int *image;
			double lambda;
			int dummyLabel;
		};

int label_dist(int value);
int smoothFn(int p1, int p2, int l1, int l2, void *data);

} /* namespace elib */

#endif /* LABELING_HPP_ */
