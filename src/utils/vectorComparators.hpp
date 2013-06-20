/*
 * vectorComparators.hpp
 *
 *  Created on: Jun 14, 2013
 *      Author: kthierbach
 */

#ifndef VECTORCOMPARATORS_HPP_
#define VECTORCOMPARATORS_HPP_

#include "../lib/glm/glm.hpp"

namespace elib{
class VectorComparators
{
	public:
		bool operator()(glm::ivec3 v1, glm::ivec3 v2)
		{
			return (v1.x <= v2.x && v1.y < v2.y);
		}
};
}

#endif /* VECTORCOMPARATORS_HPP_ */
