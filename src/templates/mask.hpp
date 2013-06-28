/*
 * mask.hpp
 *
 *  Created on: Jun 12, 2013
 *      Author: kthierbach
 */

#ifndef MASK_HPP_
#define MASK_HPP_

#include <iostream>
#include <set>
#include <stdint.h>
#include <stdlib.h>
#include <unordered_map>
#include <vector>

#include "glm/glm.hpp"
#include "image.hpp"

namespace elib{

template <class Point>
class Mask
{
	public:
		Mask()
		{
			points = new std::vector<Point>();
			outline = new std::vector<Point>();
		}
		Mask(const Mask& other)
		{
			typename std::vector<Point>::iterator it;

			this->points = new std::vector<Point>();
			for(it=other.points->begin(); it!=other.points->end(); ++it)
			{
				this->points->push_back(*it);
			}
			this->outline = new std::vector<Point>();
			for(it=other.outline->begin(); it!=other.outline->end(); ++it)
			{
				this->outline->push_back(*it);
			}
		}
		Mask(const Mask&& other) : Mask()
		{
			swap(*this,other);
		}
		virtual ~Mask()
		{
			delete points;
			delete outline;
		}
		Mask& operator=(Mask other)
		{
			swap(*this,other);
			return *this;
		}
		void addPoint(Point p)
		{
			points->push_back(p);
		}
		std::vector<Point>* getMask()
		{
			return points;
		}
		std::vector<Point>* getOutline()
		{
			return NULL;
		}
		Image<int32_t> toImage(uint32_t rank, uint32_t *dimensions)
		{
			Image<int32_t> image;
			int32_t *image_data;
			int32_t pixel;
			typename std::vector<Point>::iterator it;

			image = Image<int32_t>(rank, dimensions, bit_depth, 1);
			image_data = image.getData();
			for(it=points->begin(); it!=points->end(); ++it)
			{
				for(int32_t i=0; i<rank; ++i)
				{
					if((*it)[i] < 0 || (*it)[i]>=dimensions[i])
					{
						//throw GenericException("Mask doesn't fit into the image");
						return Image<int32_t>();
					}
				}
				pixel = Mask::getPixel(*it, dimensions);
				image_data[pixel] = 1;
			}
			return image;
		}
		static Image<int32_t> masksToImage(uint32_t rank, uint32_t *dimensions, std::unordered_map<int32_t, Mask<Point>* > *masks)
		{
			Image<int32_t> image;
			int32_t *image_data, label, pixel;
			typename std::unordered_map<int32_t, Mask<Point>* >::iterator it;
			typename std::vector<Point> *points;
			typename std::vector<Point>::iterator pt_it;

			image = Image<int32_t>(rank, dimensions, bit_depth, 1);
			image_data = image.getData();
			for(it = masks->begin(); it!=masks->end(); ++it)
			{
				label = it->first;
				points = it->second->getPoints();
				for(pt_it=points->begin(); pt_it!=points->end(); ++pt_it)
				{
					for(int32_t i=0; i<rank; ++i)
					{
						if((*pt_it)[i] < 0 || (*pt_it)[i]>dimensions[i])
						{
							//throw GenericException("Mask doesn't fit into the image");
							return Image<int32_t>();
						}
					}
					pixel = Mask::getPixel(*pt_it, dimensions);
					image_data[pixel] = label;
				}
			}
			return image;
		}
		std::vector<Point> getBoundingBox()
		{
			return getBox(points);
		}
		std::string getBoxMask()
		{
			return boxMask(getBoundingBox(), points);
		}
		std::vector<Point>* getPoints()
		{
			return points;
		}
		int32_t getSize()
		{
			return (int32_t)points->size();
		}
	private:
		std::vector<Point> *points;
		std::vector<Point> *outline;
		const static uint32_t bit_depth = 16;

		friend void swap(Mask<Point>& first, Mask<Point>& second) // nothrow
		{
			// enable ADL (not necessary in our case, but good practice)
			using std::swap;
			// by swapping the members of two classes,
			// the two classes are effectively swapped
			swap(first.points, second.points);
			swap(first.outline, second.outline);
		}

		static bool less(glm::ivec2 p1, glm::ivec2 p2)
		{
			return (p1[1] < p2[1] && p1[0] < p2[0]);
		}
		static bool less(glm::ivec3 p1, glm::ivec3 p2)
		{
			return (p1[2] < p2[2] && p1[1] < p2[1] && p1[0] < p2[0]);
		}
		static inline int32_t getPixel(glm::ivec2 p, uint32_t *dimensions)
		{
			return p[1]*dimensions[0]+p[0];
		}
		static inline int32_t getPixel(glm::ivec3 p, uint32_t *dimensions)
		{
			return p[2]*dimensions[0]*dimensions[1] + p[1]*dimensions[0] + p[0];
		}
		std::vector<glm::ivec2> getBox(std::vector<glm::ivec2> *points)
		{
			std::vector<glm::ivec2> box;
			std::vector<glm::ivec2>::iterator it;
			int32_t minX = INT32_MAX, minY = INT32_MAX, maxX = 0, maxY = 0;
			for(it=points->begin(); it!=points->end(); ++it)
			{
				minX = std::min(minX, it->x);
				minY = std::min(minY, it->y);
				maxX = std::max(maxX, it->x);
				maxY = std::max(maxY, it->y);
			}
			box.push_back(glm::ivec2(minX, minY));
			box.push_back(glm::ivec2(maxX+1, maxY+1));
			return box;
		}
		std::vector<glm::ivec3> getBox(std::vector<glm::ivec3> *points)
		{
			std::vector<glm::ivec3> box;
			std::vector<glm::ivec3>::iterator it;
			int32_t minX = INT32_MAX, minY = INT32_MAX, minZ = INT32_MAX, maxX = 0, maxY = 0, maxZ = 0;
			for(it=points->begin(); it!=points->end(); ++it)
			{
				minX = std::min(minX, it->x);
				minY = std::min(minY, it->y);
				minZ = std::min(minZ, it->z);
				maxX = std::max(maxX, it->x);
				maxY = std::max(maxY, it->y);
				maxZ = std::max(maxZ, it->z);
			}
			box.push_back(glm::ivec3(minX, minY, minZ));
			box.push_back(glm::ivec3(maxX+1, maxY+1, maxZ+1));
			return box;
		}
		std::string boxMask(std::vector<glm::ivec2> boundingBox, std::vector<glm::ivec2> *points)
		{
			glm::ivec2 p, p2;
			std::vector<char> tmp;
			std::vector<char>::iterator cit;
			std::vector<glm::ivec2>::iterator it;
			uint32_t dimensions[2];
			int32_t pixel;
			std::string mask = "";

			p = boundingBox[1] - boundingBox[0];
			dimensions[0] = p[0];
			dimensions[1] = p[1];
			tmp = std::vector<char>(p[0]*p[1], '0');
			for(it=points->begin(); it!=points->end(); ++it)
			{
				pixel = getPixel((*it)-boundingBox[0], dimensions);
				tmp[pixel] = '1';
			}
			for(cit=tmp.begin(); cit!=tmp.end(); ++cit)
			{
				mask += *cit;
			}
			return mask;
		}
};

} /* namespace elib */

#endif /* MASK_HPP_ */

