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
		bool operator()(glm::ivec2 v1, glm::ivec2 v2)
		{
			if(v1.y < v2.y)
			{
				return true;
			}
			else if(v1.y == v2.y)
			{
				if(v1.x <= v2.x)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			return false;
		}
};
}

#endif /* VECTORCOMPARATORS_HPP_ */
