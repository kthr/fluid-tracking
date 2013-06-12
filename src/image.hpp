/*
 * Image.hpp
 *
 *  Created on: May 27, 2013
 *      Author: kthierbach
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <stdint.h>
#include <stdlib.h>

template <typename type>
class Image
{
	public:
		Image(uint32_t rank, uint32_t *dimensions, uint32_t channels, uint32_t bit_depth)
		{
			this->rank = rank;
			this->dimensions  = new uint32_t[rank];
			std::copy(dimensions, dimensions+rank, this->dimensions);
			this->channels = channels;
			this->bit_depth = bit_depth;
			this->flattened_length = channels;
			for(uint32_t i=0; i<rank; ++i)
			{
				this->flattened_length*dimensions[i];
			}
			this->data = new type[flattened_length];
		}
		Image(cimage *image)
		{
			this->rank = (uint32_t)image->rank;
			std::copy(image->dimensions, image->dimensions+this->rank, this->dimensions);
			this->channels = (uint32_t)image->channels;
			this->bit_depth = (uint32_t)image->bit_depth;
			this->flattened_length = (uint32_t)image->flattened_length;
			this->data = new type[this->flattened_length];
			std::copy(image->data, image->data+this->flattened_length, this->data);
		}
		virtual ~Image()
		{
			delete dimensions;
			delete data;
		}
	private:
		uint32_t 	*dimensions,
					bit_depth,
					channels,
					flattened_length,
					rank;
		type 		*data;
};

#endif /* IMAGE_HPP_ */
