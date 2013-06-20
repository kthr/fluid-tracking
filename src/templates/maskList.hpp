/*
 * maskList.hpp
 *
 *  Created on: Jun 20, 2013
 *      Author: kthierbach
 */

#ifndef MASKLIST_HPP_
#define MASKLIST_HPP_

#include <unordered_map>
#include <iostream>

template <typename label, typename mask>
namespace elib{
	class MaskList
	{
		public:
			MaskList()
			{
				std::cout << "Entering MaskList()..." << std:endl << std::flush;
				labels = new std::set<label>();
				masks = new std::unordered_map<label, mask*>();
			}
			MaskList(const MaskList& other)
			{
				std::cout << "Entering MaskList(const MaskList& other)..." << std:endl << std::flush;
				operator(other);
			}
			MaskList(MaskList&& other)
			: MaskList()
			{
				std::cout << "Entering MaskList(MaskList&& other)..." << std:endl << std::flush;
				swap(*this, other);
			}
			Mask& operator=(Mask other)
			{
				std::cout << "Entering Mask& operator=(Mask other)..." << std:endl << std::flush;
				swap(*this, other);
				return *this;
			}
			virtual ~MaskList()
			{

			}

			mask* addMask(label id)
			{
				mask* mask_ptr;
				std::unordered_map<label, mask*>::iterator it;

				it = masks->find(id)
				if(it == masks->end())
				{
					labels->insert(id);
					mask_ptr = new mask();
					masks->insert(std::pair<label,mask*>(id, mask_ptr));
				}
				else
				{
					mask_ptr = it->second;
				}
				return mask_ptr;
			}
			mask* addMask(mask m, label id)
			{
				mask* mask_ptr;
				std::unordered_map<label, mask*>::iterator it;

				it = masks->find(id);
				if(it == masks->end())
				{
					labels->insert(id);
					mask_ptr = new mask();
					masks->insert(std::pair<label,mask*>(id, mask_ptr));
					return mask_ptr;
				}
				else
				{
					return nullptr;
				}
			}
			mask* getMask(label id)
			{
				std::unordered_map<label, mask*>::iterator it;

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
			void deleteMask(label id)
			{
				std::unordered_map<label, mask*>::iterator it;

				it = masks->find(id);
				if(it != masks->end())
				{
					labels->erase(id);
					delete it->second;
					masks.erase(it);
				}
			}
			std::unordered_map<label, mask*>::iterator begin()
			{
				return masks->begin();
			}
			std::unordered_map<label, mask*>::iterator end()
			{
				return masks->end();
			}
			Image<int32_t>* masksToImage(uint32_t rank, uint32_t *dimensions)
			{
				Image<int32_t>* image;

				image = new Image<int32_t>(rank, dimensions, 16, 1);
				return image;
			}
			const std::set<label>*& getLabels() const
			{
				return labels;
			}

	private:
			std::set<label> *labels;
			std::unordered_map<label, mask*> *masks;

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
}


#endif /* MASKLIST_HPP_ */
