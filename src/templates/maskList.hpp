/*
 * maskList.hpp
 *
 *  Created on: Jun 20, 2013
 *      Author: kthierbach
 */

#ifndef MASKLIST_HPP_
#define MASKLIST_HPP_

#include <iostream>
#include <memory>
#include <set>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "mask.hpp"

namespace elib{

template <class Label, class Point>
class MaskList
{
	public:
		MaskList() noexcept
		{
		}
		MaskList(int rank, const std::vector<int> &dimensions) : rank(rank)
		{
			this->dimensions = std::vector<int>(dimensions.begin(), dimensions.end());
		}
		MaskList(const MaskList& other)
		{
			this->rank = other.rank;
			this->dimensions = std::vector<int>(other.dimensions.begin(), other.dimensions.end());
			labels.insert(other.labels.begin(), other.labels.end());
			this->masks = other.masks;
		}
		MaskList(MaskList&& other) noexcept
			: rank(other.rank), dimensions(std::move(other.dimensions)),
			  labels(std::move(other.labels)), masks(std::move(other.masks))
		{
		}
		MaskList& operator=(MaskList &other)
		{
			swap(*this, other);
			return *this;
		}
		MaskList& operator=(MaskList &&other)
		{
			swap(*this, other);
			return *this;
		}
		virtual ~MaskList()
		{
		}

		void addMask(std::shared_ptr<Mask<Point> > &shared_mask_ptr, Label id)
		{
			auto it = masks.find(id);
			if(it == masks.end())
			{
				labels.insert(id);
				shared_mask_ptr = std::shared_ptr<Mask<Point>>(new Mask<Point>(rank, dimensions));
				masks.insert(std::pair<Label,std::shared_ptr<Mask<Point>>>(id, shared_mask_ptr));
			}
			else
			{
				shared_mask_ptr = it->second;
			}
		}
		void addMask(std::shared_ptr<Mask<Point> > &shared_mask_ptr, Label id, Mask<Point> &m)
		{
			auto it = masks.find(id);
			if(it == masks.end())
			{
				labels.insert(id);
				shared_mask_ptr = new Mask<Point>(m);
				masks.insert(std::pair<Label,std::shared_ptr<Mask<Point>>>(id, shared_mask_ptr));
			}
			else
			{
				shared_mask_ptr = it->second;
			}
		}
		void addMask(Label id, Mask<Point> &m)
		{
			auto it = masks.find(id);
			if(it == masks.end())
			{
				labels.insert(id);
				std::shared_ptr<Mask<Point>> shared_mask_ptr = std::shared_ptr<Mask<Point>>(new Mask<Point>(m));
				masks.insert(std::pair<Label,std::shared_ptr<Mask<Point>>>(id, shared_mask_ptr));
			}
		}
		void addMasks(MaskList list)
		{
			labels.insert(list.getLabels()->begin(), list.getLabels()->end());
			masks.insert(list.begin(),list.end());
		}
		std::shared_ptr<Mask<Point>> getMask(Label id)
		{
			auto it = masks.find(id);
			if(it != masks.end())
			{
				return it->second;
			}
			else
			{
				return nullptr;
			}
		}
		void deleteMask(Label id)
		{
			auto it = masks.find(id);
			if(it != masks.end())
			{
				labels.erase(id);
				masks.erase(it);
			}
		}
		void deleteMasks(std::vector<Label> &for_deletion)
		{
			auto dit = for_deletion.begin();
			while(dit != for_deletion.end())
			{
				this->deleteMask(*dit);
				++dit;
			}
		}
		void deleteSparseRepresentations()
		{
			for(auto i = masks.begin(); i != masks.end(); ++i)
			{
				i->second->deleteSparseRepresentation();
			}
		}
		Mask<Point> fuse()
		{
			Mask<Point> mask(this->rank, this->dimensions);
			const std::vector<Point> *points;

			for(auto i = masks.begin(); i!= masks.end(); ++i)
			{
				points = i->second->getPoints();
				for(auto j = points->begin(); j != points->end(); ++j)
				{
					mask.addPoint(*j);
				}
			}
			return mask;
		}
		typename std::unordered_map<Label, std::shared_ptr<Mask<Point>>>::iterator begin()
		{
			return masks.begin();
		}
		typename std::unordered_map<Label, std::shared_ptr<Mask<Point>>>::iterator end()
		{
			return masks.end();
		}
		Image<int> toImage()
		{
			Image<int> image;
			std::shared_ptr<Mask<Point>> mask_ptr;
			int *image_data, label, pixel;

			image = Image<int>(rank, dimensions, bit_depth, 1);
			image_data = image.getData();
			for(auto it = labels.begin(); it!=labels.end(); ++it)
			{
				label = *it;
				mask_ptr = this->getMask(label);
				if(mask_ptr!=nullptr)
				{
					auto points = mask_ptr->getPoints();
					for(auto pt_it=points->begin(); pt_it!=points->end(); ++pt_it)
					{
						for(int i=0; i<rank; ++i)
						{
							if((*pt_it)[i] < 0 || (*pt_it)[i]>dimensions[i])
							{
								//throw GenericException("Mask doesn't fit into the image");
								return Image<int>();
							}
						}
						pixel = mask_ptr->getPixel(*pt_it);
						image_data[pixel] = label;
					}
				}
				else
				{
					std::cerr << "mask with label " << label << " not found!";
				}
			}
			return image;
		}
		void relabel(int offset = 0)
		{
			typename std::unordered_map<Label, std::shared_ptr<Mask<Point>>> tmp;
			int id = 1+offset;

			labels.clear();
			for(auto it = masks.begin(); it!=masks.end(); ++it)
			{
				tmp.insert(std::pair<Label, std::shared_ptr<Mask<Point>>>(id, it->second));
				labels.insert(id);
				++id;
			}
			masks = tmp;
		}
		const std::set<Label>* getLabels() const
		{
			return &labels;
		}
		int getSize() const
		{
			return masks.size();
		}
		int getRank() const
		{
			return rank;
		}
		const std::vector<int>* getDimensions() const
		{
			return &dimensions;
		}
		void setDimensions(const std::vector<int> &dimensions)
		{
			this->dimensions = std::vector<int>(dimensions.begin(), dimensions.end());
			for(auto i = masks.begin(); i != masks.end(); ++i)
			{
				i->second->setDimensions(dimensions);
			}
		}
		void setRank(int rank)
		{
			this->rank = rank;
			for(auto i = masks.begin(); i != masks.end(); ++i)
			{
				i->second->setRank(rank);
			}
		}
		void setOrigin(Point origin)
		{
			for(auto i = masks.begin(); i != masks.end(); ++i)
			{
				i->second->setOrigin(origin);
			}
		}
		std::string toString()
		{
			std::stringstream ss;
			typename std::set<Label>::iterator it;

			ss << "Size of list: " << labels.size() << std::endl;
			ss << "Rank: " << rank << std::endl;
			ss << "Dimensions: ";
			for(int i=0; i<rank; ++i)
			{
				ss << dimensions[i] << " ";
			}
			ss << std::endl;
			for(it=labels.begin(); it!=labels.end(); ++it)
			{
				ss << "\t label: " << *it << " size: " << masks.find(*it)->second->getSize() << std::endl;
			}
			return ss.str();
		}

private:
		int rank = 0;
		std::vector<int> dimensions;
		const static int bit_depth = 16;
		std::set<Label> labels;
		std::unordered_map<Label, std::shared_ptr<Mask<Point>>> masks;

		friend void swap(MaskList& first, MaskList& second) // nothrow
		{
			using std::swap;

			swap(first.labels, second.labels);
			swap(first.masks, second.masks);
			first.rank = second.rank;
			swap(first.dimensions, second.dimensions);
		}
};

} /* end namespace elib */


#endif /* MASKLIST_HPP_ */
