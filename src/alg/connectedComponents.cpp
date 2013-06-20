/*
 * connectedComponents.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "connectedComponents.hpp"
#include "../lib/glm/glm.hpp"

using elib::ConnectedComponents;
using elib::Image;

std::vector<glm::ivec2> ConnectedComponents::SMALL_2D({glm::ivec2(-1,0), glm::ivec2(0,-1), glm::ivec2(1,0), glm::ivec2(0,1)});
std::vector<glm::ivec2> ConnectedComponents::LARGE_2D({glm::ivec2(-1,0), glm::ivec2(-1,-1), glm::ivec2(0,-1), glm::ivec2(1,-1), glm::ivec2(1,0), glm::ivec2(1,1), glm::ivec2(0,1), glm::ivec2(-1,1)});

ConnectedComponents::ConnectedComponents()
{
}

ConnectedComponents::~ConnectedComponents()
{
	// TODO Auto-generated destructor stub
}

Image<int32_t>* ConnectedComponents::getComponents(Image<int32_t> *image)
{
	Image<int32_t> *label_image, *tmp_image;
	int32_t *tmp_data, *label_data;
	uint32_t width, height, *dimensions;

	tmp_image = new Image<int32_t>(*image);
	tmp_data = tmp_image->getData();
	label_image = new Image<int32_t>(image->getRank(), image->getDimensions(), 16, 1);
	label_data = label_image->getData();

	if(image->getRank() == 2) //2d-image
	{
		dimensions=image->getDimensions();
		width = dimensions[0];
		height = dimensions[1];
		std::queue<glm::ivec2 > indices;
		std::vector<glm::ivec2 > *neighbours;
		glm::ivec2 index, neighbour;
		int pixel;

		if(connectivity == SMALL_CONNECTIVITY)
		{
			neighbours = &(ConnectedComponents::SMALL_2D);
		}
		else
		{
			neighbours = &(ConnectedComponents::LARGE_2D);
		}

		for(int j=0; j<height; ++j)
		{
			for(int i=0; i<width; ++i)
			{
				pixel = j*width+i;
				if(tmp_data[pixel] > 0)
				{
					tmp_data[pixel] = 0;
					label_data[pixel] = label;
					index = glm::ivec2(i,j);
					addNeigbours(&indices, neighbours, index, image->getRank(), image->getDimensions());
					while(!indices.empty())
					{
						index = indices.front();
						indices.pop();
						pixel = index.y*width + index.x;
						if(tmp_data[pixel] > 0)
						{
							tmp_data[pixel] = 0;
							label_data[pixel] = label;
							addNeigbours(&indices, neighbours, index,  image->getRank(), image->getDimensions());
						}
					}
					label++;
				}

			}
		}
	}
	else //3d-image
	{

	}

	delete tmp_image;
	return label_image;
}
