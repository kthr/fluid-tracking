/*
 * connectedComponents.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "connectedComponents.hpp"

using elib::ConnectedComponents;

short ConnectedComponents::connectivity;

ConnectedComponents::ConnectedComponents()
{
	 connectivity = SMALL_CONNECTIVITY;
}

ConnectedComponents::~ConnectedComponents()
{
	// TODO Auto-generated destructor stub
}

Image<int32_t>* ConnectedComponents::getComponents(Image<int32_t> *image)
{
	Image<int32_t> *label_image, *tmp_image;
	int32_t label, *data, *label_data;
	uint32_t width, height, depth;

	label = 1;
	tmp_image = new Image<int32_t>(image);
	data = tmp_image->getData();
	label_image = new Image(image->getRank(), image->getDimensions(), 16, 1);
	label_data = label_image->getData();;

	if(image->getRank() == 2) //2d-image
	{
		width = image->dimensions[0];
		height = image->dimensions[1];
		std::queue<glm::vec2 > indices;
		std::vector<glm::vec2 > *neighbours;
		glm::vec2 index, neighbour;
		int pixel;

		if(connectivity == SMALL_CONNECTIVITY)
		{
//			neighbours = &(ConnectedComponents::small_2d);
		}
		else
		{
//			neighbours = &(ConnectedComponents::large_2d);
		}

		for(int j=0; j<height; ++j)
		{
			for(int i=0; i<width; ++i)
			{
				pixel = j*width+i;
				if(data[pixel] > 0)
				{
					data[pixel] = 0;
					label_data[pixel] = label;
					index = glm::vec2(i,j);
					addNeigbours(&indices, neighbours, index, width, height);
					while(!indices.empty())
					{
						index = indices.front();
						indices.pop();
						pixel = index.y*width + index.x;
						if(data[pixel] > 0)
						{
							data[pixel] = 0;
							label_data[pixel] = label;
							addNeigbours(&indices, neighbours, index, width, height);
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

inline void ConnectedComponents::addNeigbours(std::queue<glm::vec2> *indices, std::vector<glm::vec2> *neighbours, glm::vec2 index, int width, int height)
{
	std::vector<glm::vec2>::iterator it;
	glm::vec2 neighbour;

	for(it = neighbours->begin(); it!=neighbours->end(); ++it)
	{
		neighbour = index+*it;
		if(!(neighbour.x < 0 || neighbour.x >= width || neighbour.y < 0 || neighbour.y >= height))
		{
			indices->push(neighbour);
		}
	}
}
