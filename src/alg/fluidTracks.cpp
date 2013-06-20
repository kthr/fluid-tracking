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

FluidTracks::FluidTracks()
{
	divisions = new vector<glm::ivec2>;
	frames = new vector<unordered_map<int32_t, mask2D* > >();
}
FluidTracks::FluidTracks(vector<string> *images, vector<string> *flows, Parameters *params)
: params(params), images(images), flows(flows)
{
	divisions = new vector<glm::ivec2>;
	frames = new vector<unordered_map<int32_t, mask2D* > >();
}

FluidTracks::FluidTracks(vector<string> *images, Parameters *params)
: params(params), images(images)
{
	divisions = new vector<glm::ivec2>;
	frames = new vector<unordered_map<int32_t, mask2D* > >();
}

FluidTracks::~FluidTracks()
{
	vector<unordered_map<int32_t, mask2D* > >::iterator fit;
	for(fit=frames->begin(); fit!=frames->end(); ++fit)
	{
		clearMasks(*fit);
	}
	delete divisions;
	delete frames;
}

void FluidTracks::addAppearingObjects(unordered_map<int32_t, mask2D* > &masks)
{
	ComponentsMeasurements cm;
	ConnectedComponents cc;
	Image<int32_t> *label_image;
	mask2D* new_objects;
	unordered_map<int32_t, mask2D*> new_object_masks;
	unordered_map<int32_t, mask2D*>::iterator it;

	it = masks.find(1);
	if(it != masks.end())
	{
		new_objects = it->second;
		std::cout << "label 1 mask size: " << new_objects->getSize() << std::endl << std::flush;
		label_image = cc.getComponents(new_objects->toImage(initial->getRank(), initial->getDimensions()));
		Image<int32_t>::saveImage("/Users/kthierbach/new_labels.png",label_image);
		cm = ComponentsMeasurements(label_image);
		cm.getMasks(new_object_masks);
		applySizeConstraints(new_object_masks);
		for(it=new_object_masks.begin(); it!=new_object_masks.end(); ++it)
		{
			std::cout << "new mask size: " << it->second->getSize() << " label: " << id_counter << std::endl << std::flush;
			masks.insert(std::pair<int32_t, mask2D* >(id_counter++, new mask2D(*(it->second))));
		}
		masks.erase(1);
		delete label_image;
	}
}

void FluidTracks::applySizeConstraints(unordered_map<int32_t, mask2D* > &masks)
{
	unordered_map<int32_t, mask2D* >::iterator it;
	uint32_t size;

	it = masks.begin();
	while(it != masks.end())
	{
		size = it->second->getSize();
		if(size < min_object_size || size > max_object_size)
			it = masks.erase(it);
		else
			++it;
	}
}

unordered_map<int32_t, mask2D* > FluidTracks::detectDivisions(unordered_map<int32_t, mask2D* > &masks)
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int32_t> *label_image;
	int32_t max;
	unordered_map<int32_t, mask2D* > objects, tmp_masks;
	unordered_map<int32_t, mask2D* >::iterator it, it2;

	cc = ConnectedComponents();
	it= masks.begin();
	while(it!=masks.end())
	{
		std::cout << "mask size: " << it->second->getSize() << std::endl << std::flush;
		label_image = cc.getComponents(it->second->toImage(initial->getRank(), initial->getDimensions()));
		max = label_image->max();
		if(max == 1)
		{
			tmp_masks.insert(*it);
		}
		if(max > 1)
		{
			cm = ComponentsMeasurements(label_image);
			cm.getMasks(objects);
			applySizeConstraints(objects);
			if(objects.size() == 1)//no real division
			{
				tmp_masks.insert(std::pair<int32_t, mask2D* >(it->first,objects.begin()->second));
			}
			if(objects.size() > 1)
			{
				for(it2=objects.begin(); it2!=objects.end(); ++it2)
				{
					divisions->push_back(glm::ivec2(it2->first, id_counter));
					tmp_masks.insert(std::pair<int32_t, mask2D* >(id_counter++,it2->second));
				}
			}
		}
		delete label_image;
		++it;
	}
	return tmp_masks;
}

void FluidTracks::track()
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int32_t> *old_label, *propagated_label;
	Labeling lbg;
	unordered_map<int32_t, mask2D*> masks;
	try
	{
		cc.setLabelOffset(2);
		if(initial_mask_image.compare("") == 0)
		{
			initial = cc.getComponents(Image<int32_t>::openImage((*images)[0]));
			Image<int32_t>::saveImage("/Users/kthierbach/test.png",initial);
			cm = ComponentsMeasurements(initial);
			cm.getMasks(masks);
			applySizeConstraints(masks);
			std::cout << "size:" << masks.size() << std::endl << std::flush;
			initial = Mask<glm::ivec3>::masksToImage(initial->getRank(), initial->getDimensions(), &masks);
			Image<int32_t>::saveImage("/Users/kthierbach/initial.png",initial);
		}
		else
		{
			initial = cc.getComponents(Image<int32_t>::openImage(initial_mask_image));
		}
		old_label = new Image<int32_t>(*initial);
		Image<int32_t>::saveImage("/Users/kthierbach/old_label.png",old_label);
		cm = ComponentsMeasurements(old_label);
		params->setIntParam(0, cm.getNumLabels()+2);
		clearMasks(masks);
		cm.getMasks(masks);
		frames->push_back(masks);
		id_counter = *(--cm.getLabels().end())+1;
		for(int i=1; i<images->size(); ++i)
		{
			if(flows != NULL)
			{
				//displace old label with flow i-1
			}
			propagated_label = lbg.labeling(old_label, Image<int32_t>::openImage((*images)[i]), params);
			Image<int32_t>::saveImage("/Users/kthierbach/prop.png",propagated_label);
			delete old_label;
			cm = ComponentsMeasurements(propagated_label);
			if(include_appearing)
			{
				unordered_map<int32_t, mask2D*>::iterator bla;
				masks.clear();
				cm.getMasks(masks);
				for(bla=masks.begin(); bla!=masks.end(); ++bla)
					std::cout << "cm mask size: " << bla->second->getSize() << " label: " << bla->first << std::endl << std::flush;
				addAppearingObjects(masks);
				for(bla=masks.begin(); bla!=masks.end(); ++bla)
					std::cout << "add mask size: " << bla->second->getSize() << " label: " << bla->first << std::endl << std::flush;
				masks = detectDivisions(masks);
				for(bla=masks.begin(); bla!=masks.end(); ++bla)
					std::cout << "div mask size: " << bla->second->getSize() << std::endl << std::flush;

			}
			else
			{
				cm.deleteMask(1);
				masks.clear();
				cm.getMasks(masks);
				masks = detectDivisions(masks);
			}
			frames->push_back(masks);
			old_label  = elib::Mask<glm::ivec3>::masksToImage(initial->getRank(), initial->getDimensions(), &masks);
		}
	}
	catch(class IOException &e)
	{
		std::cout << e.what();
		return;
	}
}

	void FluidTracks::clearMasks(unordered_map<int32_t, mask2D*> &masks)
	{
		unordered_map<int32_t, mask2D* >::iterator mit;
		for(mit=masks.begin(); mit!=masks.end(); ++mit)
		{
//			delete mit->second;
		}
		masks.clear();
	}

} /* namespace elib */
