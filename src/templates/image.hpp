/*
 * Image.hpp
 *
 *  Created on: May 27, 2013
 *      Author: kthierbach
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <algorithm>
#include <stdint.h>

#include "../c_wrapper.h"

namespace elib{

template <typename type>
class Image
{
	public:
		Image(uint32_t rank, uint32_t *dimensions, uint32_t bit_depth, uint32_t channels)
		: bit_depth(bit_depth), channels(channels), rank(rank)
		{
			this->dimensions  = new uint32_t[rank];
			std::copy(dimensions, dimensions+rank, this->dimensions);
			this->flattened_length = channels;
			for(uint32_t i=0; i<rank; ++i)
			{
				this->flattened_length*=dimensions[i];
			}
			this->data = new type[flattened_length];
		}
		Image(cimage *image)
		: bit_depth(image->bit_depth), channels(image->channels), flattened_length(image->flattened_length), rank(image->rank)
		{
			std::copy(image->dimensions, image->dimensions+rank, dimensions);
			data = new type[flattened_length];
			std::copy(image->data, image->data+flattened_length, data);
		}
		Image(const Image &image)
		: bit_depth(image.getBitDepth()), channels(image.getChannels()), flattened_length(image.getFlattenedLength()), rank(image.getRank())
		{
			dimensions = new uint32_t[rank];
			std::copy(image.getDimensions(), image.getDimensions()+rank, dimensions);
			data = new type[flattened_length];
			std::copy(image.getData(), image.getData()+flattened_length, data);
		}
		virtual ~Image()
		{
			delete dimensions;
			delete data;
		}
		cimage* to_cimage()
		{
			cimage *new_image;

			new_image = (cimage*)malloc(sizeof(cimage));
			new_image->bit_depth = this->getBitDepth();
			new_image->channels = this->getChannels();
			new_image->rank = this->getRank();
			new_image->dimensions = (mint*)malloc(sizeof(mint)*this->getRank());
			std::copy(this->getDimensions(), this->getDimensions()+this->getRank(), new_image->dimensions);
			new_image->flattened_length = this->getFlattenedLength();
			new_image->data = (mint*)malloc(sizeof(mint)*new_image->flattened_length);
			std::copy(this->getData(), this->getData()+new_image->flattened_length, new_image->data);
			new_image->shared = 0;

			return new_image;
		}
		uint32_t getBitDepth() const
		{
			return bit_depth;
		}

		uint32_t getChannels() const
		{
			return channels;
		}

		type* getData() const
		{
			return data;
		}

		uint32_t* getDimensions() const
		{
			return dimensions;
		}

		uint32_t getFlattenedLength() const
		{
			return flattened_length;
		}

		uint32_t getRank() const
		{
			return rank;
		}

		void setData(type* data)
		{
			std::copy(data, data+flattened_length, this->data);
		}

	private:
		uint32_t 	*dimensions,
					bit_depth,
					channels,
					flattened_length,
					rank;
		type 		*data;
};

} /* namespace elib */

#endif /* IMAGE_HPP_ */
