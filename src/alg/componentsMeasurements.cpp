/*
 * componentsMeasurements.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "componentsMeasurements.hpp"

#include "connectedComponents.hpp"

using elib::ComponentsMeasurements;
using elib::Image;
using std::unordered_map;

ComponentsMeasurements::ComponentsMeasurements()
{
	labels = new std::set<int32_t>();
	masks = new MaskList2D();
}
ComponentsMeasurements::ComponentsMeasurements(const ComponentsMeasurements& other)
{
	operator=(other);
}
elib::ComponentsMeasurements::ComponentsMeasurements(ComponentsMeasurements&& other)
: ComponentsMeasurements::ComponentsMeasurements()
{
	swap(*this,other);
}
ComponentsMeasurements::ComponentsMeasurements(Image<int32_t> label_image)
{
	this->label_image = label_image;
	labels = new std::set<int32_t>();
	masks = new MaskList2D();
	init();
}
ComponentsMeasurements::~ComponentsMeasurements()
{
	delete labels;
	delete masks;
}
elib::MaskList2D ComponentsMeasurements::getMasks()
{
	return *masks;
}

void ComponentsMeasurements::init()
{
	Image<int32_t>* tmp_image;
	int32_t *tmp_data;
	uint32_t *dimensions, width, height, label;

	tmp_image = new Image<int32_t>(label_image);
	tmp_data = tmp_image->getData();

	if (label_image.getRank() == 2) //2d-image
	{
		dimensions = label_image.getDimensions();
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
					Mask2D* mask_ptr;
					label = tmp_data[pixel];
					tmp_data[pixel] = 0;
					mask_ptr = masks->addMask(label);
					mask_ptr->addPoint(glm::ivec2(i, j));

					index = glm::ivec2(i, j);
					ConnectedComponents::addNeigbours(&indices, neighbours, index, label_image.getRank(), label_image.getDimensions());
					while (!indices.empty())
					{
						index = indices.front();
						indices.pop();
						pixel = index.y * width + index.x;
						if (tmp_data[pixel] == label)
						{
							tmp_data[pixel] = 0;
							mask_ptr->addPoint(glm::ivec2(index.x, index.y));
							ConnectedComponents::addNeigbours(&indices, neighbours, index, label_image.getRank(), label_image.getDimensions());
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
