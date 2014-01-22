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

MaskList<int, glm::ivec2> FluidTracks::assignLabels(Image<int> &image, MaskList<int, glm::ivec2> &old_labels, MaskList<int, glm::ivec2> &segmentation)
{
	using namespace boost::numeric::ublas;

	MaskList<int, glm::ivec2> labeled_masks(old_labels.getRank(), *old_labels.getDimensions());
	std::vector<int> labels;
	compressed_matrix<int> adjacency(segmentation.getSize(), old_labels.getSize());
	std::vector<int> 	segmentation_index_to_id(segmentation.getSize()),
						old_labels_index_to_id(old_labels.getSize());

	int old_labels_index = 0;
	for(auto j=old_labels.begin(); j!=old_labels.end(); ++j, ++old_labels_index)
	{
		old_labels_index_to_id[old_labels_index] = j->first;
	}
	int segmentation_index = 0;
	for(auto i=segmentation.begin(); i!=segmentation.end(); ++i, ++segmentation_index)
	{
		segmentation_index_to_id[segmentation_index] = i->first;
		old_labels_index = 0;
		for(auto j=old_labels.begin(); j!=old_labels.end(); ++j, ++old_labels_index)
		{
			if((*i->second).multiply((*j->second)).getSize() > 0)
			{
				adjacency.push_back(segmentation_index, old_labels_index,1);
			}
		}
	}

//	int index=1;
//	std::cout << "adjacency: #segmentations=" << segmentation.getSize() << " #old_labels=" << old_labels.getSize() << std::endl;
//	std::cout << "\trow-wise:" << std::endl;
//	for (auto it1 = adjacency.begin1(); it1 != adjacency.end1(); it1++)
//	{
//		if(it1.begin() == it1.end())
//		{
//			std::cout << "\t  " << index++ << ".  (" << it1.index1() << ",:) = empty" << std::endl;
//		}
//		for (auto it2 = it1.begin(); it2 != it1.end(); it2++)
//		{
//			std::cout << "\t  " << index++ << ".  (" << it2.index1() << "," << it2.index2() << ") = ";
//			std::cout << *it2 << std::endl;
//		}
//	}
//	std::cout << "\tcolumn-wise:" << std::endl;
//	index = 1;
//	for (auto it1 = adjacency.begin2(); it1 != adjacency.end2(); it1++)
//	{
//		if(it1.begin() == it1.end())
//		{
//			std::cout << "\t  "<< index++ << ".  (:," << it1.index2()  << ") = empty" << std::endl;
//		}
//		for (auto it2 = it1.begin(); it2 != it1.end(); it2++)
//		{
//			std::cout << "\t  " << index++ << ".  (" << it2.index1() << "," << it2.index2() << ") = ";
//			std::cout << *it2 << std::endl;
//		}
//	}
//
//	std::cout << "\tassociations:" << std::endl;
//	index = 1;
//	for (auto it1 = adjacency.begin1(); it1 != adjacency.end1(); ++it1)
//	{
//		if(it1.begin() == it1.end())
//		{
//			std::cout << "\t  " << index++ << ".  is added as new cell" << std::endl;
//		}
//		else
//		{
//			int count = 0;
//			for (auto it2 = it1.begin(); it2 != it1.end(); it2++)
//			{
//				count++;
//				matrix_column<compressed_matrix<int>> column(adjacency, it2.index1());
//				for(auto it3=column.begin(); it3!=column.end(); ++it3)
//				{
//					count++;
//				}
//			}
//			if(count == 2)
//			{
//				std::cout << "\t  " << index++ << ".  is associated with cell " << it1.begin().index2()+1 << "." << std::endl;
//			}
//			else
//			{
//				std::cout << "\t  " << index++ << ".  association is ambiguous." << std::endl;
//			}
//		}
//
//	}

	for (auto it1 = adjacency.begin1(); it1 != adjacency.end1(); ++it1)
	{
		std::pair<std::list<int>,std::list<int>> tmp = getAssociations(adjacency, it1.index1());
		if(tmp.second.empty() && include_appearing) // add new object
		{
			labeled_masks.addMask(id_counter++, *segmentation.getMask(segmentation_index_to_id[it1.index1()]));
		}
		else if(tmp.first.empty() && tmp.second.size()==1) // unique association
		{
			labeled_masks.addMask(old_labels_index_to_id[tmp.second.front()], *segmentation.getMask(segmentation_index_to_id[it1.index1()]));
		}
		else if(!tmp.first.empty() && tmp.second.size()==1) // division
		{
			for(auto i = tmp.first.begin(); i!=tmp.first.end(); ++i)
			{
				divisions->push_back(glm::ivec2(old_labels_index_to_id[tmp.second.front()], id_counter));
				labeled_masks.addMask(id_counter++, *segmentation.getMask(segmentation_index_to_id[*i]));
			}
		}
		else
		{
			MaskList<int, glm::ivec2> 	masks(old_labels.getRank(), *old_labels.getDimensions()),
										label_masks,
										tmp_masks;
			int index = 1;
			for(auto i = tmp.first.begin(); i != tmp.first.end(); ++i)
			{
				masks.addMask(index++, *segmentation.getMask(segmentation_index_to_id[*i]));
			}
			for(auto i = tmp.second.begin(); i != tmp.second.end(); ++i)
			{
				masks.addMask(index++, *old_labels.getMask(old_labels_index_to_id[*i]));
				label_masks.addMask(old_labels_index_to_id[*i], *old_labels.getMask(old_labels_index_to_id[*i]));
			}
			Mask<glm::ivec2> fused = masks.fuse();
			std::vector<glm::ivec2> bounding_box = fused.getBoundingBox();
			Image<int> label_image = old_labels.masksToImage();
			std::shared_ptr<Image<int>> image_part = image.imageTake(bounding_box[0], bounding_box[1]),
										label_tmp_part = label_image.imageTake(bounding_box[0], bounding_box[1]);
			ComponentsMeasurements cm = ComponentsMeasurements(*label_tmp_part);
			tmp_masks = cm.getMasks();
			masks = MaskList<int, glm::ivec2>(tmp_masks.getRank(), *tmp_masks.getDimensions());
			for(auto i = tmp.second.begin(); i != tmp.second.end(); ++i)
			{
				masks.addMask(old_labels_index_to_id[*i], *tmp_masks.getMask(old_labels_index_to_id[*i]));
			}
			Image<int> label_image_part = masks.masksToImage();
			params->addParameter("NumberLabels", int(tmp.second.size()+2));
			Labeling lbg;
			Image<int> *new_label_image = lbg.labeling(label_image_part, *image_part, *params);
			label_image_part.saveImage("/home/kthierbach/Desktop/host/label_part.png");
			image_part->saveImage("/home/kthierbach/Desktop/host/image_part.png");
			new_label_image->saveImage("/home/kthierbach/Desktop/host/new_label_part.png");
			cm = ComponentsMeasurements(*new_label_image);
			masks = cm.getMasks();
			masks.deleteMask(1);
			detectDivisions(masks);
			masks.setRank(old_labels.getRank());
			masks.setDimensions(*old_labels.getDimensions());
			masks.setOrigin(bounding_box[0]);
			labeled_masks.addMasks(masks);

			delete new_label_image;
		}

//		std::cout << "\t  " << index++ << " is associated with seg: ";
//		for(auto i=tmp.first.begin(); i!=tmp.first.end(); ++i)
//		{
//			std::cout << *i << " ";
//		}
//		std::cout << "\t label: ";
//		for(auto i=tmp.second.begin(); i!=tmp.second.end(); ++i)
//		{
//			std::cout << *i << " ";
//		}
//		std::cout << std::endl;

	}

	return labeled_masks;
}

std::pair<std::list<int>,std::list<int>> FluidTracks::getAssociations(boost::numeric::ublas::compressed_matrix<int> &adjacency, int id)
{
	using namespace boost::numeric::ublas;
	std::pair<std::list<int>,std::list<int>> labels;
	std::list<int> row_labels, column_labels, tmp;
	int current_row, current_column;

	current_row = id;
	row_labels=getRowAssociations(adjacency, id);
	labels.second.insert(labels.second.end(), row_labels.begin(), row_labels.end());
	while(!row_labels.empty())
	{
		current_column =  row_labels.front();
		row_labels.pop_front();
		tmp = getColumnAssociations(adjacency, current_column);
		tmp.remove(current_row);
		labels.first.insert(labels.first.end(), tmp.begin(), tmp.end());
		column_labels.insert(column_labels.end(), tmp.begin(), tmp.end());
		while(!column_labels.empty())
		{
			current_row = column_labels.front();
			tmp = getRowAssociations(adjacency, current_row);
			tmp.remove(current_column);
			column_labels.pop_front();
			labels.second.insert(labels.second.end(), tmp.begin(), tmp.end());
			row_labels.insert(row_labels.end(), tmp.begin(), tmp.end());
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
	Image<int> *tmp, image, old_label, *propagated_label;
	Labeling lbg;
	MaskList<int, glm::ivec2> masks;

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
		Image<int> emptyLabel(initial.getRank(), *(initial.getDimensions()), 16, initial.getChannels());
		propagated_label = graphcut(initial, *params);
		initial = *propagated_label;
		delete propagated_label;
		initial = cc.getComponents(initial);
		cm = ComponentsMeasurements(initial);
		masks = cm.getMasks();
		applySizeConstraints(masks);
		masks.relabel(1);
		initial = masks.masksToImage();
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
		applySizeConstraints(masks);
		masks.relabel(1);
		initial = masks.masksToImage();
		delete tmp;
	}
	old_label = Image<int>(initial);
	cm = ComponentsMeasurements(old_label);
	masks = cm.getMasks();
	frames->push_back(masks);
	id_counter = *(--masks.getLabels()->end())+1;
	VectorArray2D va;
	if(verbosity)
	{
		std::cout << "frame: " << 0 << ", #objects: " << masks.getSize() << std::endl;
	}
	for(unsigned int i=1; i<images->size(); ++i)
	{
		if(verbosity && i%10 == 0)
		{
			std::cout << "frame: " << i << ", #objects: " << masks.getSize() << std::endl;
		}
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
		propagated_label = graphcut(image, *params);
		cc.setLabelOffset();
		Image<int> components = cc.getComponents(*propagated_label);
		cm = ComponentsMeasurements(components);
		masks = cm.getMasks();
		applySizeConstraints(masks);
		masks = assignLabels(image, frames->back(), masks);
		frames->push_back(masks);
		old_label = masks.masksToImage();

		delete tmp;
		delete propagated_label;
	}
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
