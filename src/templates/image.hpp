/*
 * Image.hpp
 *
 *  Created on: May 27, 2013
 *      Author: kthierbach
 */

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "CImg.h"
#include "glm/glm.hpp"
#include "utils/vector2D.hpp"
#include "utils/vectorArray2D.hpp"

namespace elib{

template <typename type>
class Image
{
	public:
		Image()
		{
		}
		Image(const Image &other)
		: bit_depth(other.bit_depth), channels(other.channels), flattened_length(other.flattened_length), rank(other.rank)
		{
			data = new type[flattened_length];
			dimensions=std::vector<int>(other.dimensions.begin(), other.dimensions.end());
			std::copy(other.data, other.data+flattened_length, data);
		}
		Image(Image &&other)
		: dimensions(std::move(other.dimensions)), bit_depth(other.bit_depth), channels(other.channels),
		  flattened_length(other.flattened_length), rank(other.rank), data(std::move(other.data))
		{
			other.data=nullptr;
		}
		Image(int rank, const std::vector<int> &dimensions, int bit_depth, int channels)
		: bit_depth(bit_depth), channels(channels), rank(rank)
		{
			this->dimensions=std::vector<int>(dimensions.begin(), dimensions.end());
			this->flattened_length = channels;
			for(auto i=dimensions.begin(); i!=dimensions.end(); ++i)
			{
				this->flattened_length*=(*i);
			}
			this->data = new type[flattened_length];
			std::fill_n(this->data, flattened_length, 0);
		}
//		explicit Image(cimage *image)
//		: bit_depth(image->bit_depth), channels(image->channels), flattened_length(image->flattened_length), rank(image->rank)
//		{
//			dimensions = new int[rank];
//			std::copy(image->dimensions, image->dimensions+rank, dimensions);
//			data = new type[flattened_length];
//			std::copy(image->data, image->data+flattened_length, data);
//		}
		explicit Image(cimg_library::CImg<type> *image)
		{
			if(image->depth() == 1)
			{
				rank = 2;
				dimensions=std::vector<int>({image->width(), image->height()});
//				dimensions = new int[rank];
//				dimensions[0] = image->width();
//				dimensions[1] = image->height();
			}
			else
			{
				rank = 3;
				dimensions=std::vector<int>({image->width(), image->height(), image->depth()});
//				dimensions = new int[rank];
//				dimensions[0] = image->width();
//				dimensions[1] = image->height();
//				dimensions[2] = image->depth();
			}
			channels = 1;
			if(image->spectrum()>1)
			{
				cimg_library::CImg<type> tmp = image->get_channel(0);
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
		virtual ~Image()
		{
			delete[] data;
		}

		Image& operator=(Image &other)
		{
			swap(*this,other);
			return *this;
		}
		Image& operator=(Image &&other)
		{
			dimensions=std::move(other.dimensions);
			bit_depth=other.bit_depth;
			channels=other.channels;
			flattened_length=other.flattened_length;
			rank=other.rank;
			data=other.data;
			other.data=nullptr;
			return *this;
		}
		type min()
		{
			type minimum;

			minimum = data[0];
			for(int i=1; i<flattened_length; ++i)
				minimum = std::min(minimum, data[i]);
			return minimum;
		}
		type max()
		{
			type maximum;

			maximum = data[0];
			for(int i=1; i<flattened_length; ++i)
				maximum = std::max(maximum, data[i]);
			return maximum;
		}
		void multiply(double value)
		{
			for(int i=0; i< flattened_length; ++i)
			{
				data[i] *= value;
			}
		}
		void displaceByVectorField(VectorArray2D &field)
		{
			Image tmp = Image<type>(this->getRank(),*(this->getDimensions()),this->getBitDepth(),this->getChannels());
			type *data = tmp.getData();

			if(rank==2)
			{
				int width = dimensions[0],
					height = dimensions[1],
					max_index_width = width-1,
					max_index_height = height-1,
					k, l;
				Vector2D v;
				for(int j=0; j<height; ++j)
				{
					for(int i=0; i<width; ++i)
					{
						v = field.get(i,j);
						l = i-round(v.x);
						k = j-round(v.y);
						if(0<=l && l<width && 0<=k && k<height)
						{
							data[i+j*width] = this->data[l+k*width];
						}
						else
						{
							using std::min;
							using std::max;
							data[i+j*width] = this->data[min(max_index_width,max(0,l))+min(max_index_height,max(0,k))*width];
//							data[i+j*width] = 0.;
						}
					}
				}
				*this = tmp;
			}
		}
		std::shared_ptr<Image<type>> imageTake(glm::ivec2 upperLeft, glm::ivec2 downRight) const
		{
			if(rank ==  2 && upperLeft.x <= downRight.x && upperLeft.y <= downRight.y && upperLeft.x >= 0 &&
					downRight.x <= dimensions[0] && downRight.y <= dimensions[1])
			{
				std::vector<int> new_dimensions;
				int new_width = downRight.x-upperLeft.x;
				new_dimensions.push_back(new_width);
				new_dimensions.push_back(downRight.y-upperLeft.y);
				std::shared_ptr<Image<type>> part = std::shared_ptr<Image<type>>(new Image<type>(this->rank, new_dimensions, this->bit_depth, this->channels));

				int pixel = upperLeft.x + upperLeft.y*this->getWidth();
				std::copy(this->data + pixel, this->data + pixel + new_width, part->data);
				for(int i=1; i<new_dimensions[1]; ++i)
				{
					pixel += this->getWidth();
					std::copy(this->data + pixel, this->data + pixel + new_width, part->data + i*new_width);
				}
				return part;
			}
			else
			{
				return nullptr;
			}
		}
		static std::shared_ptr<Image<type>> openImage(std::string file_name)
		{
			try
			{
				cimg_library::CImg<type> img(file_name.c_str());
				return std::shared_ptr<Image<type>>(new Image<type>(&img));
			}
			catch(cimg_library::CImgException &e)
			{
				return nullptr;
			}
		}
		void saveImage(std::string file_name) const
		{
			Image<type>::saveImage(file_name, this);
		}
		static void saveImage(std::string file_name, const Image<type> *image)
		{
			try
			{
				if(image->getRank() == 2)
				{
					int width, height, spectrum;

					width = (*image->getDimensions())[0];
					height = (*image->getDimensions())[1];
					spectrum = image->getChannels();
					cimg_library::CImg<type> img(width, height, 1, spectrum);
					std::copy(image->getData(), image->getData()+image->getFlattenedLength(), img.data());
					img.save(file_name.c_str());
				}
				else
				{
				}
			}
			catch(cimg_library::CImgException &e)
			{
				std::string message = std::string("ERROR: Failed to save image at: ") + file_name;
			}
		}
		int getBitDepth() const
		{
			return bit_depth;
		}
		int getChannels() const
		{
			return channels;
		}
		type* getData() const
		{
			return data;
		}
		const std::vector<int>* getDimensions() const
		{
			return &dimensions;
		}
		int getFlattenedLength() const
		{
			return flattened_length;
		}
		int getRank() const
		{
			return rank;
		}
		void setData(const type* data)
		{
			std::copy(data, data+flattened_length, this->data);
		}
		int getWidth() const
		{
			if(rank>0)
				return dimensions[0];
			else
				return 0;
		}
		int getHeight() const
		{
			if(rank>1)
				return dimensions[1];
			else
				return 0;
		}
		int getDepth() const
		{
			if(rank>2)
				return dimensions[2];
			else
				return 1;
		}

	private:
		std::vector<int> dimensions;
		int		bit_depth = 0,
				channels = 0,
				flattened_length = 0,
				rank = 0;
		type 	*data = nullptr;

		friend void swap(Image& first,Image& second)
		{
			using std::swap;

			swap(first.dimensions, second.dimensions);
			swap(first.bit_depth, second.bit_depth);
			swap(first.channels, second.channels);
			swap(first.flattened_length, second.flattened_length);
			swap(first.rank, second.rank);
			swap(first.data, second.data);
		}
};

} /* namespace elib */

#endif /* IMAGE_HPP_ */
