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

#include "templates/image.hpp"

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
		template <typename Point>
		inline static void addNeigbours(std::queue<Point> *indices, std::vector<Point> *neighbours, Point index, int width, int height)
		{
			typename std::vector<Point>::iterator it;
			Point neighbour;

			for(it = neighbours->begin(); it!=neighbours->end(); ++it)
			{
				neighbour = index+*it;
				if(!(neighbour.x < 0 || neighbour.x >= width || neighbour.y < 0 || neighbour.y >= height))
				{
					indices->push(neighbour);
				}
			}
		}
	private:

};

} /* namespace elib */

#endif /* CONNECTEDCOMPONENTS_HPP_ */
