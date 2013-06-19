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
#include <string>

#include "../c_wrapper.h"
#include "../lib/CImg.h"

namespace elib{

using cimg_library::CImg;
using cimg_library::CImgIOException;
using std::string;

template <typename type>
class Image
{
	public:
		Image();
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
			dimensions = new uint32_t[rank];
			std::copy(image->dimensions, image->dimensions+rank, dimensions);
			data = new type[flattened_length];
			std::copy(image->data, image->data+flattened_length, data);
		}
		Image(CImg<type> *image)
		{
			if(image->depth() == 1)
			{
				rank = 2;
				dimensions = new uint32_t[rank];
				dimensions[0] = image->width();
				dimensions[1] = image->height();
			}
			else
			{
				rank = 3;
				dimensions = new uint32_t[rank];
				dimensions[0] = image->width();
				dimensions[1] = image->height();
				dimensions[2] = image->depth();
			}
			channels = 1;
			if(image->spectrum()>1)
			{
				CImg<type> tmp = image->get_channel(0);
				flattened_length = tmp.size();
				data = new type[flattened_length];
				std::copy(tmp.data(), tmp.data() + tmp.size(), data);
			}
			else
			{
				flattened_length = image->size();
				data = new type[flattened_length];
				std::copy(image->data(), image->data() + image->size(), data);
			}
			if(image->max() > pow(2,16)-1)//32bit
				bit_depth = 32;
			if(image->max() > pow(2,8)-1)//16bit
				bit_depth = 16;
			else//8bit
				bit_depth = 8;
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
		type min()
		{
			type minimum;

			minimum = data[0];
			for(uint32_t i=1; i<flattened_length; ++i)
				minimum = std::min(minimum, data[i]);
			return minimum;
		}
		type max()
		{
			type maximum;

			maximum = data[0];
			for(uint32_t i=1; i<flattened_length; ++i)
				maximum = std::max(maximum, data[i]);
			return maximum;
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
		static Image<type>* openImage(string file_name)
		{
			try
			{
				CImg<type> img(file_name.c_str());
				return new Image<type>(&img);
			}
			catch(CImgIOException &e)
			{
				//throw IOException;
				return NULL;
			}
		}
		static void saveImage(string file_name, Image<type> *image)
		{
			try
			{
				if(image->getRank() == 2)
				{
					uint32_t width, height, spectrum;

					width = image->getDimensions()[0];
					height = image->getDimensions()[1];
					spectrum = image->getChannels();
					CImg<type> img(width, height, 1, spectrum);
					std::copy(image->getData(), image->getData()+image->getFlattenedLength(), img.data());
					img.save(file_name.c_str());
				}
				else
				{

				}
			}
			catch(CImgIOException &e)
			{
				//throw IOException;
			}
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
