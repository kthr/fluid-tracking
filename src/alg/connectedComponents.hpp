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

#include "glm/glm.hpp"
#include "templates/image.hpp"

namespace elib{

class ConnectedComponents
{
	public:
		ConnectedComponents();
		virtual ~ConnectedComponents();

		const static short SMALL_CONNECTIVITY = 0;
		const static short LARGE_CONNECTIVITY = 1;
		static std::vector<glm::ivec2> SMALL_2D, LARGE_2D;

		Image<int32_t> getComponents(Image<int32_t> image);
		short getConnectivity() const
		{
			return connectivity;
		}
		void setConnectivity(short connectivity = SMALL_CONNECTIVITY)
		{
			this->connectivity = connectivity;
		}
		int32_t getLabelOffset() const
		{
			return label;
		}
		void setLabelOffset(int32_t label = 1)
		{
			this->label = label;
		}

		template<typename Point>
		inline static void addNeigbours(std::queue<Point> *indices, std::vector<Point> *neighbours, Point index, uint32_t rank, uint32_t *dimensions)
		{
			typename std::vector<Point>::iterator it;
			Point neighbour;
			bool push;

			for(it = neighbours->begin(); it!=neighbours->end(); ++it)
			{
				neighbour = index+*it;
				push = true;
				for(uint32_t i=0; i<rank; ++i)
				{
					if(neighbour[i] < 0 || neighbour[i] >= dimensions[i])
						push = false;
				}
				if(push)
				{
					indices->push(neighbour);
				}
			}
		}
	private:
		int32_t label = 1;
		short connectivity = LARGE_CONNECTIVITY;
};

} /* namespace elib */

#endif /* CONNECTEDCOMPONENTS_HPP_ */
