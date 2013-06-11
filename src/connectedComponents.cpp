/*
 * connectedComponents.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "connectedComponents.hpp"

ConnectedComponents::ConnectedComponents()
{
	// TODO Auto-generated constructor stub
	static short connectivity = SMALL_CONNECTIVITY;
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

	tmp_image = cloneImage(image);
	label = 1;
	data = tmp_image->data;
	label_data = label_image->data;
	label_image = createImage(image);
	if(image->rank == 2) //2d-image
	{
		width = image->dimensions[0];
		height = image->dimensions[1];
		std::queue<glm::vec2<int> > indices;
		std::vector<glm::vec2<int> > neighbours;
		glm::vec2<int> index, neighbour;
		int pixel;

		neighbours.push_back(glm::vec2(-1,0));
		neighbours.push_back(glm::vec2(0,-1));
		neighbours.push_back(glm::vec2(1,0));
		neighbours.push_back(glm::vec2(0,1));
		if(connectivity == LARGE_CONNECTIVITY)
		{
			neighbours.push_back(glm::vec2(-1,-1));
			neighbours.push_back(glm::vec2(1,-1));
			neighbours.push_back(glm::vec2(1,1));
			neighbours.push_back(glm::vec2(-1,1));
		}

		for(int j=0; j<height; ++j)
		{
			for(int i=0; i<width; ++i)
			{
				pixel = j*width+i;
				if(data[pixel] == 1)
				{
					label_data[pixel] = label;
					index = glm::vec2(i,j);
					addNeigbours(indices, neighbours, index, width, height);
					while(!indices.empty())
					{
						index = indices.front();
						indices.pop();
						pixel = index.y*width * index.x;
						if(data[pixel] == 1)
						{
							data[pixel] = 0;
							label_data[pixel] = label;
							addNeigbours(indices, neighbours, index, width, height);
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

inline void ConnectedComponents::addNeigbours(std::queue<glm::vec2<int> > indices, std::vector<glm::vec2<int> > neighbours, glm::vec2<int> index, int width, int height)
{
	std::vector<glm::vec2<int> >::iterator it;
	glm::vec2<int> neighbour;

	for(it = neighbours.begin(); it!=neighbours.end(); ++it)
	{
		neighbour = index+*it;
		if(!(neighbour.x < 0 || neighbour.x > width || neighbour.y < 0 || neighbour.y > height))
		{
			indices.push(neighbour);
		}
	}
}
