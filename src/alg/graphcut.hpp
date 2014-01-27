/*
 * GraphCutImage.hpp
 *
 *  Created on: Jan 22, 2013
 *      Author: kthierbach
 */

#ifndef GRAPHCUTIMAGE_HPP_
#define GRAPHCUTIMAGE_HPP_

#include <math.h>
#include <memory>

#include "templates/image.hpp"
#include "utils/parameters.hpp"

namespace elib{

std::shared_ptr<Image<int>> graphcut(Image<int> &input_image, Parameters &params);

} /* end namespace elib */

#endif /* GRAPHCUTIMAGE_HPP_ */
