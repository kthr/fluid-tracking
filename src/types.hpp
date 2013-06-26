/*
 * types.hpp
 *
 *  Created on: Jun 19, 2013
 *      Author: kthierbach
 */

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include "templates/mask.hpp"
#include "templates/maskList.hpp"
#include "../lib/glm/glm.hpp"

namespace elib{

	typedef elib::Mask<glm::ivec2> Mask2D;
	typedef elib::MaskList<int32_t, glm::ivec2> MaskList2D;

}

#endif /* TYPES_HPP_ */
