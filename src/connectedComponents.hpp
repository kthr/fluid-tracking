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
#include "../lib/glm/glm.hpp"

class ConnectedComponents
{
	public:
		ConnectedComponents();
		virtual ~ConnectedComponents();

		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;
		const static std::vector<glm::ivec2> small_2d = {glm::ivec2(-1,0) ,glm::ivec2(0,-1), glm::ivec2(1,0), glm::ivec2(0,1)};
		const static std::vector<glm::ivec2> large_2d = {glm::ivec2(-1,0) ,glm::ivec2(0,-1), glm::ivec2(1,0), glm::ivec2(0,1), glm::ivec2(-1,-1), glm::ivec2(1,-1), glm::ivec2(1,1), glm::ivec2(-1,1)};

		static short connectivity;

		static cimage* getComponents(cimage *image);
		inline static void addNeigbours(std::queue<glm::ivec2> *indices, std::vector<glm::ivec2> *neighbours, glm::ivec2 index, int width, int height);
	private:

};

#endif /* CONNECTEDCOMPONENTS_HPP_ */
