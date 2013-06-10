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
		Image(type data, uint32_t rank, uint32_t *dimensions, uint32_t channels, uint32_t bit_depth)
		{
			this->rank = rank;
			this->dimensions  = new uint32_t[rank];
			memcpy(this->dimensions, dimensions, sizeof(uint32_t)*rank);
			this->channels = channels;
			this->bit_depth = bit_depth;
			this->flattened_length = channels;
			for(uint32_t i=0; i<rank; ++i)
			{
				this->flattened_length*dimensions[i];
			}
			this->data = new type[flattened_length];
			memcpy(this->data, data, sizeof(type)*flattened_length);
		}
		virtual ~Image()
		{
			delete dimensions;
			delete data;
		}
	private:
		uint32_t rank, *dimensions, channels, bit_depth, flattened_length;
		type *data;
};

#endif /* IMAGE_HPP_ */
