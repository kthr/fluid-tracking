/*
 * componentsMeasurements.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "componentsMeasurements.hpp"

#include "connectedComponents.hpp"
#include "glm/glm.hpp"
#include "templates/mask.hpp"

namespace elib{

ComponentsMeasurements::ComponentsMeasurements() noexcept
{
}
ComponentsMeasurements::ComponentsMeasurements(const ComponentsMeasurements& other)
{
	operator=(other);
}
ComponentsMeasurements::ComponentsMeasurements(ComponentsMeasurements&& other) noexcept
: label_image(std::move(other.label_image)), num_labels(other.num_labels),
  connectivity(other.connectivity), labels(std::move(other.labels)),
  masks(std::move(other.masks))
{
}
ComponentsMeasurements& ComponentsMeasurements::operator=(ComponentsMeasurements other)
{
	swap(*this, other);
	return *this;
}
ComponentsMeasurements::ComponentsMeasurements(Image<int> &image) : label_image(image)
{
	masks = MaskList<int, glm::ivec2>(image.getRank(), *(image.getDimensions()));
	init();
}
ComponentsMeasurements::~ComponentsMeasurements()
{
}
MaskList<int, glm::ivec2> ComponentsMeasurements::getMasks()
{
	return masks;
}

void ComponentsMeasurements::init()
{
	Image<int> tmp_image;
	int *tmp_data;
	int width, height, label;
	std::vector<int> dimensions;

	tmp_image = Image<int>(label_image);
	tmp_data = tmp_image.getData();

	if (label_image.getRank() == 2) //2d-image
	{
		dimensions = *(label_image.getDimensions());
		width = dimensions[0];
		height = dimensions[1];
		std::queue<glm::ivec2> indices;
		std::vector<glm::ivec2> *neighbours;
		glm::ivec2 index, neighbour;
		int pixel;

		if (connectivity == SMALL_CONNECTIVITY)
		{
			neighbours = &(ConnectedComponents::SMALL_2D);
		}
		else
		{
			neighbours = &(ConnectedComponents::LARGE_2D);
		}

		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				pixel = j * width + i;
				if (tmp_data[pixel] > 0)
				{
					std::shared_ptr<Mask<glm::ivec2>> shared_mask_ptr;
					label = tmp_data[pixel];
					tmp_data[pixel] = 0;
					masks.addMask(shared_mask_ptr, label);
					shared_mask_ptr->addPoint(glm::ivec2(i, j));

					index = glm::ivec2(i, j);
					ConnectedComponents::addNeigbours(&indices, neighbours, index, label_image.getRank(), dimensions);
					while (!indices.empty())
					{
						index = indices.front();
						indices.pop();
						pixel = index.y * width + index.x;
						if (tmp_data[pixel] == label)
						{
							tmp_data[pixel] = 0;
							shared_mask_ptr->addPoint(glm::ivec2(index.x, index.y));
							ConnectedComponents::addNeigbours(&indices, neighbours, index, label_image.getRank(), dimensions);
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

} /* end namespace elib */
