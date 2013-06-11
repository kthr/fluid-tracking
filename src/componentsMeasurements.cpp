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
	std::vector<std::vector<Eigen::Triplet<int32_t> > > triplets;

	labels.insert(label_image->data, label_image->data + label_image->flattened_length);
	label_data = label_image->data;
	tmp_image = cloneImage(label_image);
	tmp_data = tmp_image->data;

	if(label_image->rank == 2) //2d-image
		{
			width = label_image->dimensions[0];
			height = label_image->dimensions[1];
			std::queue<glm::vec2 > indices;
			std::vector<glm::vec2 > neighbours;
			glm::vec2 index, neighbour;
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
					if(label_data[pixel] > 0)
					{
						label = label_data[pixel];
						if(labels.insert(label).second == true)
						{
							std::vector<Eigen::Triplet<int32_t> > tmp;
							Eigen::SparseMatrix<int32_t> matrix(width,height);
							std::pair<int32_t,int32_t>
							masks.push_back(matrix);
							triplets.push_back(tmp);
							labels2masks_map.insert();
						}

						index = glm::vec2(i,j);
						ConnectedComponents::addNeigbours(&indices, neighbours, index, width, height);
						while(!indices.empty())
						{
							index = indices.front();
							indices.pop();
							pixel = index.y*width + index.x;
							if(label_data[pixel] == label)
							{
								tmp_data[pixel] = -1;
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
