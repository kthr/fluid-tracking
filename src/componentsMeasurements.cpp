/*
 * componentsMeasurements.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "componentsMeasurements.hpp"

using elib::ComponentsMeasurements;
using elib::Image;

short ComponentsMeasurements::connectivity;


ComponentsMeasurements::ComponentsMeasurements(Image<int32_t> *label_image)
{
	this->label_image = label_image;
	init();
}

ComponentsMeasurements::~ComponentsMeasurements()
{
	// TODO Auto-generated destructor stub
}

bool ComponentsMeasurements::deleteMask(int32_t label)
{
	if(labels2masks_map.erase(label))
	{
		return true;
	}
	else
	{
		return false;
	}
}

elib::Mask<glm::ivec3, elib::VectorComparators>* ComponentsMeasurements::getMask(int32_t label)
{
	return &(labels2masks_map.find(label)->second);
}

Image<int32_t>* ComponentsMeasurements::masksToImage(uint32_t rank, uint32_t *dimensions)
{
	Image<int32_t>* image;

	image = new Image<int32_t>(rank, dimensions, 16, 1);
	return image;
}

void ComponentsMeasurements::init()
{
	Image<int32_t>* tmp_image;
	int32_t *label_data, *tmp_data;
	uint32_t *dimensions, width, height, depth, label;

	labels.insert(label_image->getData(), label_image->getData() + label_image->getFlattenedLength());
	label_data = label_image->getData();
	tmp_image = new Image<int32_t>(*label_image);
	tmp_data = tmp_image->getData();

	if (label_image->getRank() == 2) //2d-image
	{
		dimensions = label_image->getDimensions();
		width = dimensions[0];
		height = dimensions[1];
		std::queue<glm::ivec2> indices;
		std::vector<glm::ivec2> *neighbours;
		glm::ivec2 index, neighbour;
		int32_t pixel;

		if (connectivity == SMALL_CONNECTIVITY)
		{
//			neighbours = &(ConnectedComponents::small_2d);
		}
		else
		{
//			neighbours = &(ConnectedComponents::large_2d);
		}

		for (int32_t j = 0; j < height; ++j)
		{
			for (int32_t i = 0; i < width; ++i)
			{
				pixel = j * width + i;
				if (label_data[pixel] > 0)
				{
					Mask<glm::ivec3, elib::VectorComparators> mask;
					tmp_data[pixel] = 0;
					label = label_data[pixel];
					if (labels.insert(label).second == true)
					{
						std::pair<int32_t, Mask<glm::ivec3, elib::VectorComparators> > map(label, mask);
						labels2masks_map.insert(map);
					}
					else
					{
						mask = labels2masks_map.find(label)->second;
					}
					mask.addPoint(glm::ivec3(i, j, 1));

					index = glm::ivec2(i, j);
					ConnectedComponents::addNeigbours(&indices, neighbours, index, width, height);
					while (!indices.empty())
					{
						index = indices.front();
						indices.pop();
						pixel = index.y * width + index.x;
						if (label_data[pixel] == label)
						{
							tmp_data[pixel] = 0;
							mask.addPoint(glm::ivec3(index.x, index.y, 1));
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
	delete tmp_image;
}
