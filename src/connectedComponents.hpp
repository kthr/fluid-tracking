/*
 * connectedComponents.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#ifndef CONNECTEDCOMPONENTS_HPP_
#define CONNECTEDCOMPONENTS_HPP_

#include <queue>
#include <vector>

#include "../lib/glm/glm.hpp"
#include "image.h"

namespace elib{

class ConnectedComponents
{
	public:
		ConnectedComponents();
		virtual ~ConnectedComponents();

		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;

		static short connectivity;

		static Image<int32_t>* getComponents(Image<int32_t> *image);
		inline static void addNeigbours(std::queue<glm::vec2> *indices, std::vector<glm::vec2> *neighbours, glm::vec2 index, int width, int height);
	private:

};

} /* namespace elib */

#endif /* CONNECTEDCOMPONENTS_HPP_ */
