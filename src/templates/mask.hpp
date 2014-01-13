/*
 * mask.hpp
 *
 *  Created on: Jun 12, 2013
 *      Author: kthierbach
 */

#ifndef MASK_HPP_
#define MASK_HPP_

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <functional>
#include <limits>
#include <memory>
#include <stdlib.h>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "circularLinkedList.hpp"
#include "glm/glm.hpp"
#include "image.hpp"

namespace elib{

template <class Point>
class Mask
{
	public:
		Mask() noexcept
		{
		}
		Mask(int rank, const std::vector<int> &dimensions) : rank(rank)
		{
			this->dimensions = std::vector<int>(dimensions.begin(), dimensions.end());
		}
		Mask(const Mask &other)
		{
			this->rank = other.rank;
			this->dimensions = std::vector<int>(other.dimensions.begin(), other.dimensions.end());
			this->points = std::vector<Point>(other.points.begin(), other.points.end());
		}
		Mask(Mask&& other) noexcept
			: rank(other.rank), dimensions(std::move(other.dimensions)),
			  points(std::move(other.points)), mask(std::move(other.mask))
		{
		}
		virtual ~Mask()
		{
			delete mask;
		}
		Mask& operator=(Mask &other)
		{
			swap(*this,other);
			return *this;
		}
		Mask& operator=(Mask &&other)
		{
			swap(*this,other);
			return *this;
		}
		const Mask operator*(const Mask &other)
		{
			namespace ub = boost::numeric::ublas;
			if(this->mask == nullptr)
			{
				this->createSparseRepresentation();
			}
			if(other->mask == nullptr)
			{
				other.createSparseRepresentation();
			}
			Mask mask(this->getRank(), this->getDimensions());
			boost::numeric::ublas::compressed_matrix<int> result = boost::numeric::ublas::element_prod(this->mask, other.mask);
			if(mask.getRank()==2)
			{
				for(auto i = result.begin2(); i!= result.end2(); ++i)
				{
					mask.addPoint(Point(i.index1(), i.index2()));
				}
			}
			return this;
		}
		void addPoint(Point p)
		{
			points.push_back(p);
			delete mask;
			mask = nullptr;
		}
		const std::vector<Point>* getMask() const
		{
			return &points;
		}
		Image<int> toImage()
		{
			Image<int> image;
			int *image_data;
			int pixel;
			typename std::vector<Point>::iterator it;

			image = Image<int>(rank, dimensions, bit_depth, 1);
			image_data = image.getData();
			for(it=points.begin(); it!=points.end(); ++it)
			{
				for(int i=0; i<rank; ++i)
				{
					if((*it)[i] < 0 || (*it)[i]>=dimensions[i])
					{
						//throw GenericException("Mask doesn't fit into the image");
						return Image<int>();
					}
				}
				pixel = Mask::getPixel(*it);
				image_data[pixel] = 1;
			}
			return image;
		}
		void getBoundingBox(std::unique_ptr<std::vector<Point> > &box) const
		{
			box = std::unique_ptr<std::vector<Point>>(new std::vector<Point>());
			getBox(box);
		}
		std::string getBoxMask()
		{
			std::unique_ptr<std::vector<Point> > box;
			getBoundingBox(box);
			return boxMask(box);
		}
		const std::vector<Point>* getPoints() const
		{
			return &points;
		}
		int getSize() const
		{
			return int(points.size());
		}
		void getOutline(std::vector<Point> &polygon, Point &centroid) const
		{
			outline(polygon, centroid);
		}
		const std::vector<int>* getDimensions() const
		{
			return &dimensions;
		}
		int getPixel(Point p)
		{
			return pixel(p);
		}
		int getRank() const
		{
			return rank;
		}

	private:
		int rank = 0;
		const static int bit_depth = 16;
		std::vector<int> dimensions;
		std::vector<Point> points;
		boost::numeric::ublas::compressed_matrix<int> *mask = nullptr;

		friend void swap(Mask<Point>& first, Mask<Point>& second) // nothrow
		{
			using std::swap;
			swap(first.rank, second.rank);
			swap(first.dimensions, second.dimensions);
			swap(first.points, second.points);
			swap(first.mask, second.mask);
		}

		void createSparseRepresentation()
		{
			mask = new boost::numeric::ublas::compressed_matrix<int>(dimensions[0], dimensions[1], points.size());
			for(auto i=points.begin(); i!=points.end(); ++i)
			{
				mask->push_back((*i).x, (*i).y, 1);
			}
		}
		inline int pixel(glm::ivec2 p)
		{
			return p[1]*dimensions[0]+p[0];
		}
		inline int pixel(glm::ivec3 p)
		{
			return p[2]*dimensions[0]*dimensions[1] + p[1]*dimensions[0] + p[0];
		}
		void getBox(std::unique_ptr<std::vector<glm::ivec2> > &box) const
		{
			int minX = std::numeric_limits<int>::max(),
				minY = std::numeric_limits<int>::max(),
				maxX = 0,
				maxY = 0;
			for(auto it=points.begin(); it!=points.end(); ++it)
			{
				minX = std::min(minX, it->x);
				minY = std::min(minY, it->y);
				maxX = std::max(maxX, it->x);
				maxY = std::max(maxY, it->y);
			}
			box->push_back(glm::ivec2(minX, minY));
			box->push_back(glm::ivec2(maxX+1, maxY+1));
		}
		void getBox(std::unique_ptr<std::vector<glm::ivec3> > &box) const
		{
			int minX = std::numeric_limits<int32_t>::max(),
				minY = std::numeric_limits<int32_t>::max(),
				minZ = std::numeric_limits<int32_t>::max(),
				maxX = 0,
				maxY = 0,
				maxZ = 0;
			for(auto it=points.begin(); it!=points.end(); ++it)
			{
				minX = std::min(minX, it->x);
				minY = std::min(minY, it->y);
				minZ = std::min(minZ, it->z);
				maxX = std::max(maxX, it->x);
				maxY = std::max(maxY, it->y);
				maxZ = std::max(maxZ, it->z);
			}
			box->push_back(glm::ivec3(minX, minY, minZ));
			box->push_back(glm::ivec3(maxX+1, maxY+1, maxZ+1));
		}
		std::string boxMask(std::unique_ptr<std::vector<glm::ivec2> > &boundingBox)
		{
			glm::ivec2 p, p2;
			std::vector<char> tmp;
			std::vector<char>::iterator cit;
			std::vector<glm::ivec2>::iterator it;
			int dimensions[2];
			int pixel;
			std::string mask = "";

			p = (*boundingBox)[1] - (*boundingBox)[0];
			dimensions[0] = p[0];
			dimensions[1] = p[1];
			tmp = std::vector<char>(p[0]*p[1], '0');
			for(it=points.begin(); it!=points.end(); ++it)
			{
				pixel = getPixel((*it)-(*boundingBox)[0]);
				tmp[pixel] = '1';
			}
			for(cit=tmp.begin(); cit!=tmp.end(); ++cit)
			{
				mask += *cit;
			}
			return mask;
		}
		void outline(std::vector<glm::ivec2> &outline, glm::ivec2 &centroid) const
		{
//			std::sort(points.begin(), points.end(), VectorSortComparator);
			if (!points.empty())
			{
				if(points.size() == 1)
				{
					//prevents function from infinite loop when the mask only contains one pixel
					centroid += points.front();
					outline.push_back(points.front());
					return;
				}
				std::unordered_set<glm::ivec2, VectorHashFunctor, VectorEqualFunctor> set;
				CircularLinkedList<glm::ivec2> neighbours;
				glm::ivec2 b, c, b0, b1, tmp, c1, *previous, *current;

				set.insert(points.begin(), points.end());

				neighbours.addLast(glm::ivec2(-1, 0));
				neighbours.addLast(glm::ivec2(-1, -1));
				neighbours.addLast(glm::ivec2(0, -1));
				neighbours.addLast(glm::ivec2(1, -1));
				neighbours.addLast(glm::ivec2(1, 0));
				neighbours.addLast(glm::ivec2(1, 1));
				neighbours.addLast(glm::ivec2(0, 1));
				neighbours.addLast(glm::ivec2(-1, 1));
				neighbours.setActualElement(glm::ivec2(-1, 0));

				b0 = points[0];
				for (int i = 1; i < neighbours.size(); i++)
				{
					previous = neighbours.getActualElementData();
					current = neighbours.getNext();
					tmp = glm::ivec2(b0.x + current->x, b0.y + current->y);
					if (set.find(tmp) != set.end())
					{
						b1 = tmp;
						c1 = glm::ivec2(b0.x + previous->x, b0.y + previous->y);
						outline.push_back(glm::ivec2(b0.x, b0.y));
						outline.push_back(glm::ivec2(b1.x, b1.y));
						break;
					}
				}
				b = b1;
				c = c1;
				while (true)
				{
					neighbours.setActualElement(glm::ivec2(c.x - b.x, c.y - b.y));
					for (int i = 1; i < 8; i++)
					{
						previous = neighbours.getActualElementData();
						current = neighbours.getNext();
						tmp = glm::ivec2(b.x + current->x, b.y + current->y);
						if (set.find(tmp) != set.end())
						{
							c = glm::ivec2(b.x + previous->x, b.y + previous->y);
							b = tmp;
							outline.push_back(glm::ivec2(b.x, b.y));
							break;
						}
					}
					if (b == b0)
						break;
				}
				for (unsigned int i = 0; i < outline.size(); ++i)
				{
					centroid += outline[i];
				}
				centroid /= outline.size();
			}
		}
		struct VectorHashFunctor
		{
			size_t operator()(const glm::ivec2 &v) const
			{
				return std::hash<int>()(v.x) ^ std::hash<int>()(v.y);
			}
		};
		struct VectorEqualFunctor
		{
			bool operator()(const glm::ivec2 &v1, const glm::ivec2 &v2) const
			{
				return (v1.x==v2.x && v1.y==v2.y);
			}
		};
};

} /* namespace elib */

#endif /* MASK_HPP_ */

