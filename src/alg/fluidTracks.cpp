/*
 * fluidTracks.cpp
 *
 *  Created on: Jun 17, 2013
 *      Author: kthierbach
 */

#include "fluidTracks.hpp"

#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <iostream>

#include "componentsMeasurements.hpp"
#include "connectedComponents.hpp"
#include "graphcut.hpp"
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
	frames = new vector<MaskList<int, glm::ivec2> >();
}
FluidTracks::FluidTracks(Parameters *params, vector<string> *images, vector<string> *flows)
: params(params), images(images), flows(flows)
{
	divisions = new vector<glm::ivec2>;
	frames = new vector<MaskList<int, glm::ivec2> >();
}
FluidTracks::~FluidTracks()
{
	delete divisions;
	delete frames;
}

void FluidTracks::addAppearingObjects(MaskList<int, glm::ivec2> &masks)
{
	ComponentsMeasurements cm;
	ConnectedComponents cc;
	Image<int> label_image;
	std::shared_ptr<Mask<glm::ivec2>> new_objects;
	MaskList<int, glm::ivec2> new_object_masks(initial.getRank(), *(initial.getDimensions()));

	new_objects = masks.getMask(1);
	if(new_objects != nullptr)
	{
		label_image = cc.getComponents(new_objects->toImage());
		cm = ComponentsMeasurements(label_image);
		new_object_masks = cm.getMasks();
		applySizeConstraints(new_object_masks);
		for(auto it=new_object_masks.begin(); it!=new_object_masks.end(); ++it)
		{
			masks.addMask(id_counter++, *(it->second));
		}
		masks.deleteMask(1);
	}
}

void FluidTracks::applySizeConstraints(MaskList<int, glm::ivec2> &masks)
{
	vector<int> for_deletion;
	int size;

	auto it = masks.begin();
	while(it != masks.end())
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
	masks.deleteMasks(for_deletion);

}

MaskList<int, glm::ivec2> FluidTracks::assignLabels(const Image<int> &image, MaskList<int, glm::ivec2> &old_labels, MaskList<int, glm::ivec2> &segmentations)
{
	using namespace boost::numeric::ublas;

	MaskList<int, glm::ivec2> labeled_masks(old_labels.getRank(), *old_labels.getDimensions());
	std::vector<int> labels;
	std::shared_ptr<compressed_matrix<int>> adjacency;
	std::vector<int> 	segmentation_index_to_id(segmentations.getSize()),
						old_labels_index_to_id(old_labels.getSize());
	std::shared_ptr<Mask<glm::ivec2>> seg_mask, label_mask;

	int old_labels_index = 0;
	for(auto i=old_labels.getLabels()->begin(); i!=old_labels.getLabels()->end(); ++i, ++old_labels_index)
	{
		old_labels_index_to_id[old_labels_index] = *i;
	}
	int segmentation_index = 0;
	for(auto i=segmentations.getLabels()->begin(); i!=segmentations.getLabels()->end(); ++i, ++segmentation_index)
	{
		segmentation_index_to_id[segmentation_index] = *i;
	}

	adjacency = computeAdjacency(old_labels, segmentations);

	std::vector<bool> visited(segmentations.getSize(), false);
	std::shared_ptr<Mask<glm::ivec2>> mask;

	for (auto it1 = adjacency->begin1(); it1 != adjacency->end1(); ++it1)
	{
		if(!visited[it1.index1()])
		{
			std::pair<std::list<int>,std::list<int>> tmp = getAssociations(*adjacency, it1.index1());

			if(tmp.second.empty()) // add new object or not
			{
				if(include_appearing)
				{
					mask = segmentations.getMask(segmentation_index_to_id[it1.index1()]);
					if(mask != nullptr)
					{
						labeled_masks.addMask(id_counter++, *mask);
					}
					else
					{
						std::cerr << "Error in FluidTracks::assignLabels:" << __LINE__ << std::endl;
						abort();
					}
				}
				visited[it1.index1()]=true;
			}
			else if(tmp.first.empty() && tmp.second.size()==1) // unique association
			{
				mask = segmentations.getMask(segmentation_index_to_id[it1.index1()]);
				if(mask != nullptr)
				{
					labeled_masks.addMask(old_labels_index_to_id[tmp.second.front()], *mask);
				}
				else
				{
					std::cerr << "Error in FluidTracks::assignLabels:" << __LINE__ << std::endl;
					abort();
				}
				visited[it1.index1()]=true;
			}
			else if(!tmp.first.empty() && tmp.second.size()==1) // division
			{
				for(auto i = tmp.first.begin(); i!=tmp.first.end(); ++i)
				{
					visited[*i] = true;
					divisions->push_back(glm::ivec2(old_labels_index_to_id[tmp.second.front()], id_counter));
					mask = segmentations.getMask(segmentation_index_to_id[*i]);
					if(mask != nullptr)
					{
						labeled_masks.addMask(id_counter++, *mask);
					}
					else
					{
						std::cerr << "Error in FluidTracks::assignLabels:" << __LINE__ << std::endl;
						abort();
					}
				}
				visited[it1.index1()]=true;
			}
			else // resolve ambiguities
			{
				MaskList<int, glm::ivec2> 	masks(old_labels.getRank(), *old_labels.getDimensions()),
											tmp_masks;

				mask = segmentations.getMask(segmentation_index_to_id[it1.index1()]);
				if(mask != nullptr)
				{
					visited[it1.index1()]=true;
					masks.addMask(0, *mask);
				}
				else
				{
					std::cerr << "Error in FluidTracks::assignLabels:" << __LINE__ << std::endl;
					abort();
				}
				int index = 1;
				for(auto i = tmp.first.begin(); i != tmp.first.end(); ++i)
				{
					mask = segmentations.getMask(segmentation_index_to_id[*i]);
					if(mask != nullptr)
					{
						visited[*i]=true;
						masks.addMask(index++, *mask);
					}
					else
					{
						std::cerr << "Error in FluidTracks::assignLabels:" << __LINE__ << std::endl;
						abort();
					}

				}
				for(auto i = tmp.second.begin(); i != tmp.second.end(); ++i)
				{
					mask = old_labels.getMask(old_labels_index_to_id[*i]);
					if(mask != nullptr)
					{
						masks.addMask(index++, *mask);
					}
					else
					{
						std::cerr << "Error in FluidTracks::assignLabels:" << __LINE__ << std::endl;
						abort();
					}
				}
				Mask<glm::ivec2> fused = masks.fuse();
				BoundingBox<glm::ivec2> bounding_box = fused.getBoundingBox();
				Image<int> label_image = old_labels.toImage();
				std::shared_ptr<Image<int>> image_part = image.imageTake(bounding_box),
											label_tmp_part = label_image.imageTake(bounding_box);

				ComponentsMeasurements cm = ComponentsMeasurements(*label_tmp_part);
				tmp_masks = cm.getMasks();
				masks = MaskList<int, glm::ivec2>(tmp_masks.getRank(), *tmp_masks.getDimensions());
				for(auto i = tmp.second.begin(); i != tmp.second.end(); ++i)
				{
					mask = tmp_masks.getMask(old_labels_index_to_id[*i]);
					if(mask != nullptr)
					{
						masks.addMask(old_labels_index_to_id[*i], *mask);
					}
					else
					{
						std::cerr << "Error in FluidTracks::assignLabels:" << __LINE__ << std::endl;
						abort();
					}
				}
				Image<int> label_image_part = masks.toImage();
				params->addParameter("NumberLabels", int(tmp.second.size()+2));
				Labeling lbg;
				std::shared_ptr<Image<int>> new_label_image = lbg.labeling(label_image_part, *image_part, *params);
				cm = ComponentsMeasurements(*new_label_image);
				masks = cm.getMasks();
				masks.deleteMask(1);
				detectDivisions(masks);
				masks.setRank(old_labels.getRank());
				masks.setDimensions(*old_labels.getDimensions());
				masks.setOrigin(bounding_box.getUpperLeft());
				labeled_masks.addMasks(masks);
			}
		}
	}
	return labeled_masks;
}

std::shared_ptr<boost::numeric::ublas::compressed_matrix<int>> FluidTracks::computeAdjacency(MaskList<int, glm::ivec2> &old_labels, MaskList<int, glm::ivec2> &segmentations)
{
	using namespace boost::numeric::ublas;

	std::shared_ptr<compressed_matrix<int>> adjacency = std::shared_ptr<compressed_matrix<int>>(new compressed_matrix<int>(segmentations.getSize(), old_labels.getSize()));
	std::vector<int> 	segmentation_index_to_id(segmentations.getSize()),
						old_labels_index_to_id(old_labels.getSize());
	std::shared_ptr<Mask<glm::ivec2>> seg_mask, label_mask;
	int old_labels_index = 0;
	for(auto i=old_labels.getLabels()->begin(); i!=old_labels.getLabels()->end(); ++i, ++old_labels_index)
	{
		old_labels_index_to_id[old_labels_index] = *i;
	}
	int segmentation_index = 0;
	for(auto i=segmentations.getLabels()->begin(); i!=segmentations.getLabels()->end(); ++i, ++segmentation_index)
	{
		segmentation_index_to_id[segmentation_index] = *i;
		old_labels_index = 0;
		for(auto j=old_labels.getLabels()->begin(); j!=old_labels.getLabels()->end(); ++j, ++old_labels_index)
		{
			seg_mask = segmentations.getMask(*i);
			label_mask = old_labels.getMask(*j);
			if(seg_mask->overlap(*label_mask))
			{
				adjacency->insert_element(segmentation_index, old_labels_index,1);
			}
		}
	}
	return adjacency;
}

std::pair<std::list<int>,std::list<int>> FluidTracks::getAssociations(boost::numeric::ublas::compressed_matrix<int> &adjacency, int id)
{
	using namespace boost::numeric::ublas;
	std::pair<std::list<int>,std::list<int>> labels;
	std::list<int> row_labels, column_labels, tmp;
	int current_row, current_column;
	std::vector<bool> visited_rows(adjacency.size1()), visited_columns(adjacency.size2());

	current_row = id;
	row_labels=getRowAssociations(adjacency, id);
	labels.second.insert(labels.second.end(), row_labels.begin(), row_labels.end());
	while(!row_labels.empty())
	{
		current_column =  row_labels.front();
		if(!visited_columns[current_column])
		{
			visited_columns[current_column] = true;
			row_labels.pop_front();
			tmp = getColumnAssociations(adjacency, current_column);
			tmp.remove(current_row);
			labels.first.insert(labels.first.end(), tmp.begin(), tmp.end());
			column_labels.insert(column_labels.end(), tmp.begin(), tmp.end());
		}
		else
		{
			row_labels.pop_front();
		}

		while(!column_labels.empty())
		{
			current_row = column_labels.front();
			if(!visited_rows[current_row])
			{
				visited_rows[current_row] = true;
				column_labels.pop_front();
				tmp = getRowAssociations(adjacency, current_row);
				tmp.remove(current_column);
				labels.second.insert(labels.second.end(), tmp.begin(), tmp.end());
				row_labels.insert(row_labels.end(), tmp.begin(), tmp.end());
			}
			else
			{
				column_labels.pop_front();
			}
		}
	}
	labels.first.sort();
	labels.first.unique();
	labels.second.sort();
	labels.second.unique();

	return labels;
}

std::list<int> FluidTracks::getColumnAssociations(boost::numeric::ublas::compressed_matrix<int> &adjacency, int id)
{
	using namespace boost::numeric::ublas;
	std::list<int> labels;

	matrix_column<compressed_matrix<int>> column(adjacency, id);
	for(auto i=column.begin(); i!=column.end(); ++i)
	{
		labels.push_back(i.index());
	}

	return labels;
}

std::list<int> FluidTracks::getRowAssociations(boost::numeric::ublas::compressed_matrix<int> &adjacency, int id)
{
	using namespace boost::numeric::ublas;
	std::list<int> labels;

	matrix_row<compressed_matrix<int>> row(adjacency, id);
	for(auto i=row.begin(); i!=row.end(); ++i)
	{
		labels.push_back(i.index());
	}

	return labels;
}

void FluidTracks::detectDivisions(MaskList<int, glm::ivec2> &masks)
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int> label_image;
	MaskList<int, glm::ivec2> 	objects(initial.getRank(),*(initial.getDimensions())),
								tmp_masks(initial.getRank(), *(initial.getDimensions()));
	vector<int> for_deletion;
	int max;


	auto it= masks.begin();
	while(it!=masks.end())
	{
		label_image = cc.getComponents(it->second->toImage());
		max = label_image.max();
		if(max > 1)
		{
			for_deletion.push_back(it->first);
			cm = ComponentsMeasurements(label_image);
			objects = cm.getMasks();
			applySizeConstraints(objects);
			if(objects.getSize() == 1)//no real division
			{
				tmp_masks.addMask(it->first, *(objects.begin()->second));
			}
			if(objects.getSize() > 1)
			{
				for(auto it2=objects.begin(); it2!=objects.end(); ++it2)
				{
					divisions->push_back(glm::ivec2(it->first, id_counter));
					tmp_masks.addMask(id_counter++,*(it2->second));
				}
			}
		}
		++it;
	}
	masks.deleteMasks(for_deletion);
	masks.addMasks(tmp_masks);
}

void FluidTracks::track()
{
	ConnectedComponents cc;
	ComponentsMeasurements cm;
	Image<int> old_label;
	std::shared_ptr<Image<int>> image, propagated_label, tmp;
	Labeling lbg;
	MaskList<int, glm::ivec2> masks;

	cc.setLabelOffset(2);
	if(initial_mask_image.compare("") == 0)
	{
		tmp = Image<int>::openImage((*images)[0]);
		if(tmp == nullptr)
		{
			std::cerr << "Image " << (*images)[0] << " not found!" << std::endl;
			abort();
		}
		else
		{
			initial = *tmp;
		}
		Image<int> emptyLabel(initial.getRank(), *(initial.getDimensions()), 16, initial.getChannels());
		propagated_label = graphcut(initial, *params);
		initial = *propagated_label;
		initial = cc.getComponents(initial);
		cm = ComponentsMeasurements(initial);
		masks = cm.getMasks();
		applySizeConstraints(masks);
		masks.relabel(1);
		initial = masks.toImage();
	}
	else /* open initial label image */
	{
		tmp = Image<int>::openImage(initial_mask_image);
		if(tmp == nullptr)
		{
			std::cerr << "Initial image " << initial_mask_image << " not found!" << std::endl;
			abort();
		}
		else
		{
			initial = *tmp;
		}
		cm = ComponentsMeasurements(initial);
		masks = cm.getMasks();
		applySizeConstraints(masks);
		masks.relabel(1);
		initial = masks.toImage();
	}
	old_label = Image<int>(initial);
	cm = ComponentsMeasurements(old_label);
	masks = cm.getMasks();
	frames->push_back(masks);
	id_counter = *(--masks.getLabels()->end())+1;
	VectorArray2D va;
	std::cout << "\r" << "frame: " << 0 << ", #objects: " << masks.getSize() << std::flush;
	for(unsigned int i=1; i<images->size(); ++i)
	{
		std::cout << "\r" << "frame: " << i << ", #objects: " << masks.getSize() << std::flush;
		if(flows->size() != 0)
		{
			va.load((*flows)[i-1].c_str());
			old_label.displaceByVectorField(va);
			cm = ComponentsMeasurements(old_label);
			masks = cm.getMasks();
		}
		image = Image<int>::openImage((*images)[i]);
		if(image == nullptr)
		{
			std::cerr << "Image " << (*images)[i] << " not found!" << std::endl;
			abort();
		}
		propagated_label = graphcut(*image, *params);
		cc.setLabelOffset();
		Image<int> components = cc.getComponents(*propagated_label);
		cm = ComponentsMeasurements(components);
		masks = cm.getMasks();
		applySizeConstraints(masks);
		masks = assignLabels(*image, frames->back(), masks);
		applySizeConstraints(masks);
		masks.deleteSparseRepresentations();
		frames->push_back(masks);
		old_label = masks.toImage();

	}
	std::cout << "\r" << "finished successfully" << std::string(50,' ') << std::endl;
}

Image<int>* FluidTracks::getInitial()
{
	return &initial;
}

vector<MaskList<int, glm::ivec2>>* FluidTracks::getFrames()
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
