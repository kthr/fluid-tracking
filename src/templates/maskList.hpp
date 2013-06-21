/*
 * maskList.hpp
 *
 *  Created on: Jun 20, 2013
 *      Author: kthierbach
 */

#ifndef MASKLIST_HPP_
#define MASKLIST_HPP_

#include <unordered_map>
#include <sstream>
#include <vector>

#include "mask.hpp"

namespace elib{

template <typename Label, typename Point>
class MaskList
{
	public:
		MaskList()
		{
			labels = new std::set<Label>();
			masks = new std::unordered_map<Label, Mask<Point>*>();
		}
		MaskList(const MaskList& other)
		{
			typename std::set<Label>::iterator sit;
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;

			this->labels = new std::set<Label>();
			for(sit=other.labels->begin(); sit!=other.labels->end(); ++sit)
			{
				this->labels->insert(*sit);
			}
			this->masks = new std::unordered_map<Label, Mask<Point>*>(other.masks->size());
			for(it=other.masks->begin(); it!=other.masks->end(); ++it)
			{
				this->masks->insert(std::pair<Label, Mask<Point>* >(it->first, new Mask<Point>(*(it->second))));
			}
		}
		MaskList(MaskList&& other)
		: MaskList()
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
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;
			for(it=masks->begin(); it!=masks->end(); ++it)
			{
				delete it->second;
			}
			delete labels;
			delete masks;
		}

		elib::Mask<Point>* addMask(Label id)
		{
			Mask<Point>* mask_ptr;
			typename std::unordered_map<Label, Mask<Point>*>::iterator it;

			it = masks->find(id);
			if(it == masks->end())
			{
				labels->insert(id);
				mask_ptr = new Mask<Point>();
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
		Image<int32_t> masksToImage(uint32_t rank, uint32_t *dimensions)
		{
			return Mask<Point>::masksToImage(rank, dimensions, masks);
		}
		std::set<Label>* getLabels()
		{
			return labels;
		}
		int32_t getSize()
		{
			return masks->size();
		}
		string toString()
		{
			std::stringstream ss;
			typename std::set<Label>::iterator it;

			ss << "Size of list: " << labels->size() << "\n";
			for(it=labels->begin(); it!=labels->end(); ++it)
			{
				ss << "\t label: " << *it << " size: " << masks->find(*it)->second->getSize() << "\n";
			}
			return ss.str();
		}
private:
		std::set<Label> *labels;
		std::unordered_map<Label, Mask<Point>*> *masks;
		const static uint32_t bit_depth = 16;

		friend void swap(MaskList& first, MaskList& second) // nothrow
		{
			// enable ADL (not necessary in our case, but good practice)
			using std::swap;

			// by swapping the members of two classes,
			// the two classes are effectively swapped
			swap(first.labels, second.labels);
			swap(first.masks, second.masks);
		}
};

} /* end namespace elib */


#endif /* MASKLIST_HPP_ */
