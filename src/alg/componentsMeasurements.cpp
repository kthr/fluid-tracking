/*
 * componentsMeasurements.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "componentsMeasurements.hpp"

using elib::ComponentsMeasurements;
using elib::Image;
using std::unordered_map;
using elib::VectorComparators;

ComponentsMeasurements::ComponentsMeasurements()
{
}
ComponentsMeasurements::ComponentsMeasurements(Image<int32_t> *label_image)
{
	this->label_image = label_image;
	labels = new std::set<int32_t>();
	masks = new unordered_map<int32_t, mask2D* >();
	init();
}
ComponentsMeasurements::~ComponentsMeasurements()
{
	unordered_map<int32_t, mask2D* >::iterator it;
	for(it=masks->begin(); it!=masks->end(); ++it)
	{
		delete it->second;
	}
	delete labels;
	delete masks;
}

bool ComponentsMeasurements::deleteMask(int32_t label)
{
	if(masks->erase(label))
	{
		return true;
	}
	else
	{
		return false;
	}
}
elib::mask2D* ComponentsMeasurements::getMask(int32_t label)
{
	unordered_map<int32_t, mask2D* >::iterator it;
	it = masks->find(label);
	if(it != masks->end())
		return it->second;
	else
		return nullptr;
}
Image<int32_t>* ComponentsMeasurements::masksToImage(uint32_t rank, uint32_t *dimensions)
{
	Image<int32_t>* image;

	image = new Image<int32_t>(rank, dimensions, 16, 1);
	return image;
}
unordered_map<int32_t, elib::mask2D > ComponentsMeasurements::getMasks()
{
	unordered_map<int32_t, mask2D> copy;
	unordered_map<int32_t, mask2D* >::iterator it;
	for(it=masks->begin(); it!=masks->end(); ++it)
	{
		copy.insert(std::pair<int32_t, mask2D>(it->first, *(it->second)));
	}
	return copy;
}

void ComponentsMeasurements::init()
{
	Image<int32_t>* tmp_image;
	int32_t *tmp_data;
	uint32_t *dimensions, width, height, depth, label;

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
			neighbours = &(ConnectedComponents::SMALL_2D);
		}
		else
		{
			neighbours = &(ConnectedComponents::LARGE_2D);
		}

		for (int32_t j = 0; j < height; ++j)
		{
			for (int32_t i = 0; i < width; ++i)
			{
				pixel = j * width + i;
				if (tmp_data[pixel] > 0)
				{
					mask2D* mask_ptr;
					label = tmp_data[pixel];
					tmp_data[pixel] = 0;
					if (labels->insert(label).second == true)
					{
						mask_ptr = new Mask<glm::ivec3, elib::VectorComparators>();
						std::pair<int32_t, mask2D* > map(label, mask_ptr);
						masks->insert(map);
						num_labels++;
					}
					else
					{
						mask_ptr = masks->find(label)->second;
					}
					mask_ptr->addPoint(glm::ivec3(i, j, 1));

					index = glm::ivec2(i, j);
					ConnectedComponents::addNeigbours(&indices, neighbours, index, label_image->getRank(), label_image->getDimensions());
					while (!indices.empty())
					{
						index = indices.front();
						indices.pop();
						pixel = index.y * width + index.x;
						if (tmp_data[pixel] == label)
						{
							tmp_data[pixel] = 0;
							mask_ptr->addPoint(glm::ivec3(index.x, index.y, 1));
							ConnectedComponents::addNeigbours(&indices, neighbours, index, label_image->getRank(), label_image->getDimensions());
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
