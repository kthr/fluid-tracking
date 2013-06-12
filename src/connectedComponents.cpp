/*
 * connectedComponents.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "connectedComponents.hpp"

short ConnectedComponents::connectivity;

ConnectedComponents::ConnectedComponents()
{
	 connectivity = SMALL_CONNECTIVITY;
}

ConnectedComponents::~ConnectedComponents()
{
	// TODO Auto-generated destructor stub
}

cimage* ConnectedComponents::getComponents(cimage *image)
{
	cimage *label_image, *tmp_image;
	int label, *data, *label_data;
	int width, height, depth;

	label = 1;
	tmp_image = cloneImage(image);
	data = tmp_image->data;
	label_image = createImage(image);
	label_data = label_image->data;

	if(image->rank == 2) //2d-image
	{
		width = image->dimensions[0];
		height = image->dimensions[1];
		std::queue<glm::ivec2 > indices;
		std::vector<glm::ivec2 > *neighbours;
		glm::ivec2 index, neighbour;
		int pixel;

		if(connectivity == SMALL_CONNECTIVITY)
		{
			neighbours = &(ConnectedComponents::small_2d);
		}
		else
		{
			neighbours = &(ConnectedComponents::large_2d);
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
					index = glm::ivec2(i,j);
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

	freeImage(tmp_image);
	return label_image;
}

inline void ConnectedComponents::addNeigbours(std::queue<glm::ivec2> *indices, std::vector<glm::ivec2> *neighbours, glm::ivec2 index, int width, int height)
{
	std::vector<glm::ivec2>::iterator it;
	glm::ivec2 neighbour;

	for(it = neighbours->begin(); it!=neighbours->end(); ++it)
	{
		neighbour = index+*it;
		if(!(neighbour.x < 0 || neighbour.x >= width || neighbour.y < 0 || neighbour.y >= height))
		{
			indices->push(neighbour);
		}
	}
}
