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
#include "labeling.hpp"
#include "utils/vectorArray2D.hpp"
#include "utils/utilities.hpp"

namespace elib
{

using std::vector;
using std::unordered_map;
using std::string;

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
	Image<int> label_image;
	Mask2D* new_objects;
	MaskList2D new_object_masks;
	unordered_map<int, Mask2D*>::iterator it;

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
	unordered_map<int, Mask2D* >::iterator it;
	vector<int> for_deletion;
	int size;

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
	Image<int> label_image;
	int max;
	MaskList2D objects, tmp_masks;
	unordered_map<int, Mask2D* >::iterator it, it2;
	vector<int> for_deletion;

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
					divisions->push_back(glm::ivec2(it->first, id_counter));
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
	Image<int> *tmp, image, old_label, *propagated_label;
	Labeling lbg;
	MaskList2D masks;

	lbg.setVerbosity(verbosity);
	lbg.setCycles(cycles);

	cc.setLabelOffset(2);
	if(initial_mask_image.compare("") == 0)
	{
		tmp = Image<int>::openImage((*images)[0]);
		if(tmp == nullptr)
			abort();
		else
			initial = *tmp;
		Image<int> emptyLabel(initial.getRank(), initial.getDimensions(), 16, initial.getChannels());
		params->setIntParam(0,2);
		propagated_label = lbg.labeling(&emptyLabel, &initial, params);
		initial = *propagated_label;
		delete propagated_label;
		initial = cc.getComponents(initial);
		cm = ComponentsMeasurements(initial);
		masks = cm.getMasks();
		applySizeConstraints(&masks);
		initial = masks.masksToImage(initial.getRank(), initial.getDimensions());
		delete tmp;
	}
	else /* open initial label image */
	{
		tmp = Image<int>::openImage(initial_mask_image);
		if(tmp == nullptr)
			abort();
		else
			initial = *tmp;
		cm = ComponentsMeasurements(initial);
		masks = cm.getMasks();
		masks.relabel(1);
		applySizeConstraints(&masks);
		initial = masks.masksToImage(initial.getRank(), initial.getDimensions());
		delete tmp;
	}
	old_label = Image<int>(initial);
	cm = ComponentsMeasurements(old_label);
	masks = cm.getMasks();
	frames->push_back(masks);
	id_counter = *(--masks.getLabels()->end())+1;
	VectorArray2D va;
	if(verbosity)
		std::cout << "frame: " << 0 << ", #objects: " << masks.getSize() << std::endl;
	for(unsigned int i=1; i<images->size(); ++i)
	{
		if(verbosity)
			std::cout << "frame: " << i << ", #objects: " << masks.getSize() << std::endl;
		if(flows->size() != 0)
		{
			va.load((*flows)[i-1].c_str());
			old_label.displaceByVectorField(va);
			cm = ComponentsMeasurements(old_label);
			masks = cm.getMasks();
		}
		tmp = Image<int>::openImage((*images)[i]);
		if(tmp == nullptr)
			abort();
		else
			image = *tmp;
		params->setIntParam(0, masks.getSize()+2);
		propagated_label = lbg.labeling(&old_label, &image, params);
		delete tmp;
		cm = ComponentsMeasurements(*propagated_label);
		masks = cm.getMasks();
		delete propagated_label;
		if(include_appearing)
		{
			addAppearingObjects(&masks);
		}
		else
		{
			masks.deleteMask(1);
		}
		detectDivisions(&masks);
		frames->push_back(masks);
		old_label = masks.masksToImage(initial.getRank(), initial.getDimensions());
	}
}

Image<int>* FluidTracks::getInitial()
{
	return &initial;
}

vector<MaskList2D>* FluidTracks::getFrames()
{
	return frames;
}

int FluidTracks::getMaxObjectSize() const
{
	return max_object_size;
}

void FluidTracks::setMaxObjectSize(int maxObjectSize)
{
	max_object_size = maxObjectSize;
}

} /* namespace elib */
