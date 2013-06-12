/*
 * componentsMeasurements.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "componentsMeasurements.hpp"

#include "connectedComponents.hpp"

short ComponentsMeasurements::connectivity;

ComponentsMeasurements::ComponentsMeasurements(cimage *label_image)
{
	this->label_image = label_image;
	init();
}

ComponentsMeasurements::~ComponentsMeasurements()
{
	// TODO Auto-generated destructor stub
}

void ComponentsMeasurements::init()
{
	cimage* tmp_image;
	int *label_data, *tmp_data;
	int32_t width, height, depth;
	int32_t label;

	labels.insert(label_image->data, label_image->data + label_image->flattened_length);
	label_data = label_image->data;
	tmp_image = cloneImage(label_image);
	tmp_data = tmp_image->data;

	if(label_image->rank == 2) //2d-image
		{
			width = label_image->dimensions[0];
			height = label_image->dimensions[1];
			std::queue<glm::ivec2 > indices;
			std::vector<glm::ivec2 > *neighbours;
			glm::ivec2 index, neighbour;
			int32_t pixel;

			if(connectivity == SMALL_CONNECTIVITY)
			{
				neighbours = &(ConnectedComponents::small_2d);
			}
			else
			{
				neighbours = &(ConnectedComponents::large_2d);
			}

			for(int32_t j=0; j<height; ++j)
			{
				for(int32_t i=0; i<width; ++i)
				{
					pixel = j*width+i;
					if(label_data[pixel] > 0)
					{
						tmp_data[pixel] = 0;
						label = label_data[pixel];
						if(labels.insert(label).second == true)
						{
							Mask<glm::ivec2> mask;
							std::pair<int32_t,int32_t> map(label, masks.size()-1);
							masks.push_back(mask);
							labels2masks_map.insert(map);
						}
						masks[labels2masks_map.find(label)->second].push_back(glm::ivec3(i,j,1));

						index = glm::ivec2(i,j);
						ConnectedComponents::addNeigbours(&indices, neighbours, index, width, height);
						while(!indices.empty())
						{
							index = indices.front();
							indices.pop();
							pixel = index.y*width + index.x;
							if(label_data[pixel] == label)
							{
								tmp_data[pixel] = 0;
								masks[labels2masks_map.find(label)->second].push_back(glm::ivec3(index.x,index.y,1));
								ConnectedComponents::addNeigbours(&indices, neighbours, index, width, height);
							}
						}
					}

				}
			}
		}
		else //3d-image
		{

		}
}
