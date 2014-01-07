/*
 * maskList.hpp
 *
 *  Created on: Jun 20, 2013
 *      Author: kthierbach
 */

#ifndef MASKLIST_HPP_
#define MASKLIST_HPP_

#include <set>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "mask.hpp"

namespace elib{

template <typename Label, typename Point>
class MaskList
{
	public:
		MaskList()
		{
		}
		MaskList(int rank, const int *dimensions) : rank(rank)
		{
			labels = new std::set<Label>();
			masks = new std::unordered_map<Label, Mask<Point>*>();
			this->dimensions = new int[rank];
			std::copy(dimensions, dimensions+rank, this->dimensions);
		}
		MaskList(const MaskList& other)
		{
			this->rank = other.rank;
			this->dimensions = new int[this->rank];
			std::copy(other.dimensions, other.dimensions+other.rank, this->dimensions);
			this->labels = new std::set<Label>();
			labels->insert(other.labels->begin(), other.labels->end());
			this->masks = new std::unordered_map<Label, Mask<Point>*>();
			this->masks = other.masks;
		}
		MaskList(MaskList&& other)
		{
			swap(*this, other);
		}
		elib::MaskList<Label,Point>& operator=(MaskList<Label,Point> other)
		{
			swap(*this, other);
			return *this;
		}
		virtual ~MaskList()
		{
			if(masks!=nullptr)
			{
				typename std::unordered_map<Label, Mask<Point>*>::iterator it;
				for(it=masks->begin(); it!=masks->end(); ++it)
				{
					delete it->second;
				}
			}
			delete labels;
			delete masks;
			delete[] dimensions;
		}

		elib::Mask<Point>* addMask(Label id)
		{
			Mask<Point>* mask_ptr;
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;

			it = masks->find(id);
			if(it == masks->end())
			{
				labels->insert(id);
				mask_ptr = new Mask<Point>(rank, dimensions);
				masks->insert(std::pair<Label,Mask<Point>*>(id, mask_ptr));
			}
			else
			{
				mask_ptr = it->second;
			}
			return mask_ptr;
		}
		elib::Mask<Point>* addMask(Label id, Mask<Point> &m)
		{
			Mask<Point>* mask_ptr;
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;

			it = masks->find(id);
			if(it == masks->end())
			{
				labels->insert(id);
				mask_ptr = new Mask<Point>(m);
				masks->insert(std::pair<Label,Mask<Point>*>(id, mask_ptr));
				return mask_ptr;
			}
			else
			{
				return nullptr;
			}
		}
		void addMasks(MaskList<Label,Point> *list)
		{
			typename std::unordered_map<Label, Mask<Point>*>::iterator it= list->begin();
			while(it!=list->end())
			{
				this->addMask(it->first, *(it->second));
				++it;
			}
		}
		elib::Mask<Point>* getMask(Label id)
		{
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;

			it = masks->find(id);
			if(it != masks->end())
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
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;

			it = masks->find(id);
			if(it != masks->end())
			{
				labels->erase(id);
				delete it->second;
				masks->erase(it);
			}
		}
		void deleteMasks(std::vector<Label> *for_deletion)
		{
			typename std::vector<Label>::iterator dit;

			dit = for_deletion->begin();
			while(dit != for_deletion->end())
			{
				this->deleteMask(*dit);
				++dit;
			}
		}
		typename std::unordered_map<Label, Mask<Point>*>::iterator begin()
		{
			return masks->begin();
		}
		typename std::unordered_map<Label, Mask<Point>*>::iterator end()
		{
			return masks->end();
		}
		Image<int> masksToImage(int rank, int *dimensions)
		{
			return Mask<Point>::masksToImage(rank, dimensions, masks);
		}
		void relabel(int offset = 0)
		{
			typename std::unordered_map<Label, Mask<Point>*> *tmp = new std::unordered_map<Label, Mask<Point>*>();
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;
			int id = 1+offset;

			labels->clear();
			for(it = masks->begin(); it!=masks->end(); ++it)
			{
				tmp->insert(std::pair<Label,Mask<Point>*>(id, it->second));
				labels->insert(id);
				++id;
			}
			delete masks;
			masks = tmp;
		}
		std::set<Label>* getLabels()
		{
			return labels;
		}
		int getSize()
		{
			return masks->size();
		}
		int getRank() const
		{
			return rank;
		}
		const int* getDimensions() const
		{
			return dimensions;
		}
		std::string toString()
		{
			std::stringstream ss;
			typename std::set<Label>::iterator it;

			ss << "Size of list: " << labels->size() << std::endl;
			ss << "Rank: " << rank << std::endl;
			ss << "Dimensions: ";
			for(int i=0; i<rank; ++i)
			{
				ss << dimensions[i] << " ";
			}
			ss << std::endl;
			for(it=labels->begin(); it!=labels->end(); ++it)
			{
				ss << "\t label: " << *it << " size: " << masks->find(*it)->second->getSize() << std::endl;
			}
			return ss.str();
		}

private:
		int rank = 0,
			*dimensions = nullptr;
		const static int bit_depth = 16;
		std::set<Label> *labels = nullptr;
		std::unordered_map<Label, Mask<Point>*> *masks = nullptr;

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
