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
		Image<int32_t>* toImage(uint32_t rank, uint32_t *dimensions)
		{
			Image<int32_t> *image;
			int32_t *image_data;
			int32_t pixel;
			typename std::set<Point>::iterator it;

			image = new Image<int32_t>(rank, dimensions, bit_depth, 1);
			image_data = image->getData();
			for(it=points->begin(); it!=points->end(); ++it)
			{
				for(int32_t i=0; i<rank; ++i)
				{
					if((*it)[i] < 0 || (*it)[i]>dimensions[i])
					{
						//throw GenericException("Mask doesn't fit into the image");
						return NULL;
					}
				}
				if(rank==2)
				{
					pixel = (*it)[1]*dimensions[0]+(*it)[0];
				}
				else
				{
					pixel = (*it)[2]*dimensions[0]*dimensions[1] + (*it)[1]*dimensions[0] + (*it)[0];
				}
				image_data[pixel] = (*it)[rank];
			}
			return image;
		}
		static Image<int32_t>* masksToImage(uint32_t rank, uint32_t *dimensions, std::unordered_map<int32_t, Mask<Point, Comparator> > *masks)
		{
			Image<int32_t> *image;
			int32_t *image_data, label, pixel;
			typename std::unordered_map<int32_t, Mask<Point, Comparator> >::iterator it;
			typename std::set<Point>::iterator pt_it;

			image = new Image<int32_t>(rank, dimensions, bit_depth, 1);
			image_data = image->getData();
			for(it = masks->begin(); it!=masks->end(); ++it)
			{
				label = it->first;
				typename std::set<Point, Comparator> const *points = it->second.getPoints();
				for(pt_it=points->begin(); pt_it!=points->end(); ++pt_it)
				{
					for(int32_t i=0; i<rank; ++i)
					{
						if((*pt_it)[i] < 0 || (*pt_it)[i]>dimensions[i])
						{
							//throw GenericException("Mask doesn't fit into the image");
							return NULL;
						}
						if(rank==2)
						{
							pixel = (*pt_it)[1]*dimensions[0]+(*pt_it)[0];
						}
						else
						{
							pixel = (*pt_it)[2]*dimensions[0]*dimensions[1] + (*pt_it)[1]*dimensions[0] + (*pt_it)[0];
						}
						image_data[pixel] = label;
					}
				}
			}
			return image;
		}
		const std::set<Point, Comparator>* getPoints()
		{
			return points;
		}
		Mask<Point,Comparator>& operator=(Mask<Point,Comparator> other)
		{
		    swap(*this, other);

		    return *this;
		}
	private:
		std::set<Point, Comparator> *points;
		std::set<Point, Comparator> *outline;
		const static int32_t bit_depth = 16;

		friend void swap(Mask<Point,Comparator>& first, Mask<Point,Comparator>& second) // nothrow
		{
			// enable ADL (not necessary in our case, but good practice)
			using std::swap;
			// by swapping the members of two classes,
			// the two classes are effectively swapped
			swap(first.points, second.points);
			swap(first.outline, second.outline);
		}
};

} /* namespace elib */

#endif /* MASK_HPP_ */

