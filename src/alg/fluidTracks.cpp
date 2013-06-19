/*
 * fluidTracks.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#include "fluidTracks.hpp"

#include <iostream>

#include "../exceptions/IOException.hpp"
#include "../labeling.hpp"
#include "connectedComponents.hpp"

namespace elib
{

FluidTracks::FluidTracks(vector<string> *images, vector<string> *flows, Parameters *params)
: params(params), images(images), flows(flows)
{
}

FluidTracks::FluidTracks(vector<string> *images, Parameters *params)
: params(params), images(images)
{
}

FluidTracks::~FluidTracks()
{
}

void FluidTracks::addAppearingObjects(unordered_map<int32_t, mask2D > *masks)
{
	ComponentsMeasurements cm;
	ConnectedComponents cc;
	Image<int32_t> *label_image;
	mask2D new_objects;
	unordered_map<int32_t, mask2D> new_object_masks;
	unordered_map<int32_t, mask2D>::iterator it;

	it = masks->find(1);
	if(it != masks->end())
	{
		new_objects = it->second;
		label_image = cc.getComponents(new_objects.toImage(initial->getRank(), initial->getDimensions()));
		cm = ComponentsMeasurements(label_image);
		new_object_masks = cm.getMasks();
		applySizeConstraints(&new_object_masks);
		for(it=new_object_masks.begin(); it!=new_object_masks.end(); ++it)
		{
			masks->insert(std::pair<int32_t, mask2D >(id_counter++, it->second));
		}
		masks->erase(1);
		delete label_image;
	}
}

void FluidTracks::applySizeConstraints(unordered_map<int32_t, mask2D >* masks)
{
	unordered_map<int32_t, mask2D >::iterator it;
	uint32_t size;

	it = masks->begin();
	while(it != masks->end())
	{
		size = it->second.getSize();
		if(size < min_object_size || size > max_object_size)
			it = masks->erase(it);
		else
			++it;
	}
}

unordered_map<int32_t, mask2D > FluidTracks::detectDivisions(unordered_map<int32_t, mask2D > *masks)
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int32_t> *label_image;
	int32_t max;
	unordered_map<int32_t, mask2D > objects, tmp_masks;
	unordered_map<int32_t, mask2D >::iterator it, it2;

	cc = ConnectedComponents();
	it= masks->begin();
	while(it!=masks->end())
	{
		label_image = cc.getComponents(it->second.toImage(initial->getRank(), initial->getDimensions()));
		max = label_image->max();
		if(max == 1)
		{
			tmp_masks.insert(*it);
		}
		if(max > 1)
		{
			cm = ComponentsMeasurements(label_image);
			objects = cm.getMasks();
			applySizeConstraints(&objects);
			if(objects.size() == 1)//no real division
			{
				tmp_masks.insert(std::pair<int32_t, mask2D >(it->first,objects.begin()->second));
			}
			if(objects.size() > 1)
			{
				for(it2=objects.begin(); it2!=objects.end(); ++it2)
				{
					divisions.push_back(glm::ivec2(it->first, id_counter));
					tmp_masks.insert(std::pair<int32_t, mask2D >(id_counter++,it2->second));
				}
			}
		}
		delete label_image;
		++it;
	}
	return tmp_masks;
}

FluidTracks::FluidTracks()
{
}

void FluidTracks::track()
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int32_t> *old_label, *propagated_label;
	Labeling lbg;
	unordered_map<int32_t, mask2D> masks;
	try
	{
		cc.setLabelOffset(2);
		if(initial_mask_image.compare("") == 0)
		{
			initial = cc.getComponents(Image<int32_t>::openImage((*images)[0]));
			cm = ComponentsMeasurements(initial);
			masks = cm.getMasks();
			applySizeConstraints(&masks);
			initial = Mask<glm::ivec3, VectorComparators>::masksToImage(initial->getRank(), initial->getDimensions(), &masks);
		}
		else
		{
			initial = cc.getComponents(Image<int32_t>::openImage(initial_mask_image));
		}
		params->setIntParam(0, initial->max()+1);
		old_label = new Image<int32_t>(*initial);
		cm = ComponentsMeasurements(old_label);
		frames.push_back(cm.getMasks());
		id_counter = *(--cm.getLabels().end())+1;
		for(int i=1; i<images->size(); ++i)
		{
			if(flows != NULL)
			{
				//displace old label with flow i-1
			}
			propagated_label = lbg.labeling(old_label, Image<int32_t>::openImage((*images)[i]), params);
			delete old_label;
			cm = ComponentsMeasurements(propagated_label);
			if(include_appearing)
			{
				masks = cm.getMasks();
				addAppearingObjects(&masks);
				masks = detectDivisions(&masks);
			}
			else
			{
				cm.deleteMask(1);
				masks = cm.getMasks();
				masks = detectDivisions(&masks);
			}
			frames.push_back(masks);
			old_label  = elib::Mask<glm::ivec3, VectorComparators>::masksToImage(initial->getRank(), initial->getDimensions(), &masks);
		}
	}
	catch(class IOException &e)
	{
		std::cout << e.what();
		return;
	}
}



} /* namespace elib */
