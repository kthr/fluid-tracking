/*
 * mask.hpp
 *
 *  Created on: Jun 12, 2013
 *      Author: kthierbach
 */

#ifndef MASK_HPP_
#define MASK_HPP_

#include <set>
#include <stdint.h>
#include <stdlib.h>

namespace elib{

template <typename Point, typename Comparator>
class Mask
{
	public:
		Mask()
		{
			points = new std::set<Point,Comparator>();
			outline = new std::set<Point,Comparator>();
		}
		virtual ~Mask()
		{
			delete points;
			delete outline;
		}
		bool addPoint(Point p)
		{
			return points->insert(p).second;

		}
		std::set<Point,Comparator>* getMask()
		{
			return points;
		}
		std::set<Point,Comparator>* getOutline()
		{
			return NULL;
		}
		int32_t getSize()
		{
			return (int32_t)points->size();
		}
		int32_t* toImage(int32_t rank, int32_t *dimensions)
		{
			int32_t *image,
					pixel,
					size;

			size = 1;
			for(int32_t i=0; i<rank; ++i)
			{
				size *= dimensions[i];
			}
			image = (int32_t*)malloc(sizeof(int32_t)*size);
			typename std::set<Point>::iterator it;
			for(it=points->begin(); it!=points->end(); ++it)
			{
				for(int32_t i=0; i<rank; ++i)
				{
					if(it[i] < 0 || it[i]>dimensions[i])
						return NULL;
				}
				image[pixel] = it[rank+1];
			}
			return image;
		}
	private:
		std::set<Point, Comparator> *points;
		std::set<Point, Comparator> *outline;
};

} /* namespace elib */

#endif /* MASK_HPP_ */

