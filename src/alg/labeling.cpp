/*
 * GraphCutImage.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: kthierbach
 */

#include "labeling.hpp"

#include <math.h>
#include <unordered_map>
#include <set>

#include <iostream>

#include "gco/GCoptimization.h"
#include "glm/glm.hpp"


#define GC_INFINITY 300000.

using elib::Labeling;
using elib::Image;

Image<int32_t>* Labeling::labeling(Image<int32_t>* label_image, Image<int32_t>* input_image, Parameters *input_params)
{
	uint32_t *dimensions, width, height, bit_depth, num_labels;
	double c0, c1, lambda, mu, c;
	std::set<int32_t> labels;
	std::set<int32_t>::iterator it;
	std::unordered_map<int,int> label_map;
	int32_t *label_array, label, num_pixels, *label_data, *image_data;
	Image<int32_t> *new_label_image = new Image<int32_t>(*label_image);


	dimensions = label_image->getDimensions();
	width = dimensions[0];
	height = dimensions[1];
	bit_depth = input_image->getBitDepth();
	num_labels = input_params->getIntegerParam(0);
	c0 = input_params->getDoubleParam(0);
	c1 = input_params->getDoubleParam(1);
	lambda = input_params->getDoubleParam(2);
	mu = input_params->getDoubleParam(3);
	label_data = label_image->getData();
	image_data = input_image->getData();

	labels.insert(label_image->getData(), label_image->getData()+label_image->getFlattenedLength());
	labels.insert(1); //inserts label 1
	label_array = new int32_t[labels.size()];
	std::copy(labels.begin(), labels.end(), label_array);

	for(int i=0; i<labels.size(); ++i)
	{
		label_map.insert(std::pair<int32_t,int32_t>(label_array[i],i));
	}

	try{
		GCoptimizationGridGraph *gc = new GCoptimizationGridGraph(width,height, num_labels);
		num_pixels = width*height;
		// first set up data costs individually

		c = c1*(pow(2,bit_depth)-1);
		//label for appearing objects == 1
		for (int32_t i = 0; i < num_pixels; i++ )
		{
			label = label_map.find(static_cast<float>(label_data[i]))->second;
			gc->setDataCost(i,1, static_cast<float>(2*mu*(label_dist(0-label))+fabs((double)image_data[i]-c)));
		}
		for (int l = 2; l < num_labels; l++ )
		{
			for (int32_t i = 0; i < num_pixels; i++ )
			{
				label = label_map.find(static_cast<float>(label_data[i]))->second;
				gc->setDataCost(i,l, static_cast<float>(mu*(label_dist(l-label))+fabs((double)image_data[i]-c)));
			}
		}
		//background labeling == 0
		c = c0*(pow(2,bit_depth)-1);
		for (int32_t i = 0; i < num_pixels; i++ )
		{
			label = label_map.find(static_cast<float>(label_data[i]))->second;
			gc->setDataCost(i,0, static_cast<float>(mu*(label_dist(0-label)))+fabs((double)image_data[i]-c));
		}

//		 //next set up smoothness costs individually
//				for ( int l1 = 0; l1 < numLabels; l1++ )
//					for (int l2 = 0; l2 < numLabels; l2++ )
//					{
//						gc->setSmoothCost(l1,l2,l1==l2 ? 0 : lambda);
//					}
		ForSmoothFn data;
		data.image = input_image->getData();
		data.lambda = lambda;
		gc->setSmoothCost(&smoothFn, &data);

		gc->setVerbosity(1);
		gc->expansion(20);
		for (int32_t i = 0; i < num_pixels; i++)
			new_label_image->getData()[i] = label_array[gc->whatLabel(i)];

		delete gc;
	}
	catch (GCException &e){
		e.Report();
	}
	return new_label_image;
}

float elib::smoothFn(int p1, int p2, int l1, int l2, void *data)
{
	ForSmoothFn fsf = *((ForSmoothFn*) data);
	if(l1==l2)
		return static_cast<float>(fsf.lambda*exp(-fabs(fsf.image[p1]-fsf.image[p2])));
	else
	{
		if((l1==1 && l2!=0) || (l1!=0 && l2==1))
		{
			return GC_INFINITY;
		}
		else
		{
			return static_cast<float>((label_dist(l1-l2)) + fsf.lambda*exp(-fabs(fsf.image[p1]-fsf.image[p2])));
//			return GC_INFINITY;
		}
	}
}

inline int elib::label_dist(int value)
{
	if(value==0)
		return 0;
	else
		return 1;
}
