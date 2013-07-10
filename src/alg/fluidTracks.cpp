/*
 * fluidTracks.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#include "fluidTracks.hpp"

#include <iostream>

#include "componentsMeasurements.hpp"
#include "connectedComponents.hpp"
#include "exceptions/IOException.hpp"
#include "labeling.hpp"

namespace elib
{

FluidTracks::FluidTracks()
{
	divisions = new vector<glm::ivec2>;
	frames = new vector<MaskList2D >();
}
FluidTracks::FluidTracks(Parameters *params, vector<string> *images, vector<string> *flows)
: params(params), images(images), flows(flows)
{
	divisions = new vector<glm::ivec2>;
	frames = new vector<MaskList2D >();
}
FluidTracks::~FluidTracks()
{
	delete divisions;
	delete frames;
}

void FluidTracks::addAppearingObjects(MaskList2D *masks)
{
	ComponentsMeasurements cm;
	ConnectedComponents cc;
	Image<int32_t> label_image;
	Mask2D* new_objects;
	MaskList2D new_object_masks;
	unordered_map<int32_t, Mask2D*>::iterator it;

	new_objects = masks->getMask(1);
	if(new_objects != nullptr)
	{
		label_image = cc.getComponents(new_objects->toImage(initial.getRank(), initial.getDimensions()));
		cm = ComponentsMeasurements(label_image);
		new_object_masks = cm.getMasks();
		applySizeConstraints(&new_object_masks);
		for(it=new_object_masks.begin(); it!=new_object_masks.end(); ++it)
		{
			masks->addMask(id_counter++, *(it->second));
		}
		masks->deleteMask(1);
	}
}

void FluidTracks::applySizeConstraints(MaskList2D *masks)
{
	unordered_map<int32_t, Mask2D* >::iterator it;
	vector<int32_t> for_deletion;
	uint32_t size;

	it = masks->begin();
	while(it != masks->end())
	{
		size = it->second->getSize();
		if(size < min_object_size || size > max_object_size)
		{
			for_deletion.push_back(it->first);
			++it;
		}
		else
			++it;
	}
	masks->deleteMasks(&for_deletion);

}

void FluidTracks::detectDivisions(MaskList2D *masks)
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int32_t> label_image;
	int32_t max;
	MaskList2D objects, tmp_masks;
	unordered_map<int32_t, Mask2D* >::iterator it, it2;
	vector<int32_t> for_deletion;

	it= masks->begin();
	while(it!=masks->end())
	{
		label_image = cc.getComponents(it->second->toImage(initial.getRank(), initial.getDimensions()));
		max = label_image.max();
		if(max > 1)
		{
			for_deletion.push_back(it->first);
			cm = ComponentsMeasurements(label_image);
			objects = cm.getMasks();
			applySizeConstraints(&objects);
			if(objects.getSize() == 1)//no real division
			{
				tmp_masks.addMask(it->first, *(objects.begin()->second));
			}
			if(objects.getSize() > 1)
			{
				for(it2=objects.begin(); it2!=objects.end(); ++it2)
				{
					divisions->push_back(glm::ivec2(it2->first, id_counter));
					tmp_masks.addMask(id_counter++,*(it2->second));
				}
			}
		}
		++it;
	}
	masks->deleteMasks(&for_deletion);
	masks->addMasks(&tmp_masks);
}

void FluidTracks::track()
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int32_t> image, old_label, *propagated_label;
	Labeling lbg;
	MaskList2D masks;
	try
	{
		cc.setLabelOffset(2);
		if(initial_mask_image.compare("") == 0)
		{
			try{
				initial = cc.getComponents(Image<int32_t>::openImage((*images)[0]));
			}
			catch(IOException &e)
			{
				std::cerr << e.what() << std::endl;
				throw "ERROR: Tracking couldn't be performed!";
			}
			cm = ComponentsMeasurements(initial);
			masks = cm.getMasks();
			applySizeConstraints(&masks);
			initial = masks.masksToImage(initial.getRank(), initial.getDimensions());
		}
		else
		{
			try{
				initial = cc.getComponents(Image<int32_t>::openImage(initial_mask_image));
			}
			catch(IOException &e)
			{
				std::cerr << e.what() << std::endl;
				throw "ERROR: Tracking couldn't be performed!";
			}
		}
		old_label = Image<int32_t>(initial);
		cm = ComponentsMeasurements(old_label);
		masks = cm.getMasks();
		frames->push_back(masks);
		id_counter = *(--masks.getLabels()->end())+1;
		for(int i=1; i<images->size(); ++i)
		{
			params->setIntParam(0, masks.getSize()+2);
			if(flows->size() != 0)
			{
				//displace old label with flow i-1
			}
			try{
				image = Image<int32_t>::openImage((*images)[i]);
			}
			catch(IOException e)
			{
				std::cerr << e.what() << std::endl;
				throw "ERROR: Tracking couldn't be performed!";
			}
			propagated_label = lbg.labeling(&old_label, &image, params);
			cm = ComponentsMeasurements(*propagated_label);
			delete propagated_label;
			if(include_appearing)
			{
				masks = cm.getMasks();
				addAppearingObjects(&masks);
				detectDivisions(&masks);
			}
			else
			{
				masks.deleteMask(1);
				detectDivisions(&masks);
			}
			frames->push_back(masks);
			old_label  = masks.masksToImage(initial.getRank(), initial.getDimensions());
		}
	}
	catch(class IOException &e)
	{
		std::cout << e.what();
		return;
	}
}

Image<int32_t>* FluidTracks::getInitial()
{
	return &initial;
}

vector<MaskList2D>* FluidTracks::getFrames()
{
	return frames;
}

uint32_t FluidTracks::getMaxObjectSize() const
{
	return max_object_size;
}

void FluidTracks::setMaxObjectSize(uint32_t maxObjectSize)
{
	max_object_size = maxObjectSize;
}

} /* namespace elib */
