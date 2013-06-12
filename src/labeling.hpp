/*
 * GraphCutImage.hpp
 *
 *  Created on: Jan 22, 2013
 *      Author: kthierbach
 */

#ifndef GRAPHCUTIMAGE_HPP_
#define GRAPHCUTIMAGE_HPP_

#include <math.h>
#include "gco/GCoptimization.h"
#include "c_wrapper.h"

namespace elib{

class Labeling
{
	public:
		Labeling();
		~Labeling();
		void labeling(cimage *new_label_image, cimage* label_image, cimage* input_image, parameters *input_params);

	private:
		struct ForSmoothFn
		{
			int *image;
			double lambda;
			int dummyLabel;
		};


		int smoothFn(int p1, int p2, int l1, int l2, void *data);
		int label_dist(int value);
};

} /* namespace elib */

#endif /* GRAPHCUTIMAGE_HPP_ */
