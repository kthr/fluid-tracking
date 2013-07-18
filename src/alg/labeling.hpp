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
		Image<int32_t>* labeling(Image<int32_t>* label_image, Image<int32_t>* input_image, Parameters *input_params);

		void setCycles(int cycles = 0)
		{
			this->cycles = cycles;
		}
		void setVerbosity(int verbosity = 0)
		{
			this->verbosity = verbosity;
		}

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
float smoothFn(int p1, int p2, int l1, int l2, void *data);

} /* namespace elib */

#endif /* LABELING_HPP_ */
