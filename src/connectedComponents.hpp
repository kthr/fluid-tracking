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

#include "c_wrapper.h"
#include "glm/glm.hpp"

class ConnectedComponents
{
	public:
		ConnectedComponents();
		virtual ~ConnectedComponents();

		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;

		static short connectivity;

		static cimage* getComponents(cimage *image);
	private:
		inline void addNeigbours(std::queue<glm::vec2<int> > indices, std::vector<glm::vec2<int> > neighbours, glm::vec2<int> index, int width, int height);
};

#endif /* CONNECTEDCOMPONENTS_HPP_ */
