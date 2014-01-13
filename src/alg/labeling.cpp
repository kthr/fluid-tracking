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
#include "utils/math_functions.hpp"


#define GC_INFINITY 300000.

using elib::Labeling;
using elib::Image;

Image<int>* Labeling::labeling(Image<int> &label_image, Image<int> &input_image, Parameters &input_params)
{
	int width, height, bit_depth, num_labels;
	double 	c0 = .1,
			c1 = .9,
			lambda = 1.,
			mu = 1.,
			c;
	std::set<int> labels;
	std::set<int>::iterator it;
	std::unordered_map<int,int> label_map;
	int *label_array, label, num_pixels, *label_data, *image_data;
	Image<int> *new_label_image = new Image<int>(label_image);



	if(
		isnan(num_labels = input_params.getIntegerParameter("NumberLabels")) ||
		isnan(c0 = input_params.getDoubleParameter("C0")) ||
		isnan(c1 = input_params.getDoubleParameter("C1")) ||
		isnan(lambda = input_params.getDoubleParameter("Lambda")) ||
		isnan(mu = input_params.getDoubleParameter("Mu"))
	)
	{
		return nullptr;
	}
	width = input_image.getWidth();
	height = input_image.getHeight();
	bit_depth = input_image.getBitDepth();
	label_data = label_image.getData();
	image_data = input_image.getData();

	labels.insert(label_image.getData(), label_image.getData()+label_image.getFlattenedLength());
	labels.insert(1); //inserts label 1
	label_array = new int[labels.size()];
	std::copy(labels.begin(), labels.end(), label_array);

	for(unsigned int i=0; i<labels.size(); ++i)
	{
		label_map.insert(std::pair<int,int>(label_array[i],i));
	}

	try{
		GCoptimizationGridGraph *gc = new GCoptimizationGridGraph(width,height, num_labels);
		num_pixels = width*height;
		// first set up data costs individually

		c = c1*(pow(2,bit_depth)-1);
		//label for appearing objects == 1
		for (int i = 0; i < num_pixels; i++ )
		{
			label = label_map.find(static_cast<int>(label_data[i]))->second;
			gc->setDataCost(i,1, static_cast<int>(mu*(label_dist(0-label))+fabs((double)image_data[i]-c)));
		}
		for (int l = 2; l < num_labels; l++ )
		{
			for (int i = 0; i < num_pixels; i++ )
			{
				label = label_map.find(static_cast<int>(label_data[i]))->second;
				gc->setDataCost(i,l, static_cast<int>(mu*(label_dist(l-label))+fabs((double)image_data[i]-c)));
			}
		}
		//background labeling == 0
		c = c0*(pow(2,bit_depth)-1);
		for (int i = 0; i < num_pixels; i++ )
		{
			label = label_map.find(static_cast<int>(label_data[i]))->second;
			gc->setDataCost(i,0, static_cast<int>(mu*(label_dist(0-label)))+fabs((double)image_data[i]-c));
		}

//		 //next set up smoothness costs individually
//				for ( int l1 = 0; l1 < numLabels; l1++ )
//					for (int l2 = 0; l2 < numLabels; l2++ )
//					{
//						gc->setSmoothCost(l1,l2,l1==l2 ? 0 : lambda);
//					}
		ForSmoothFn data;
		data.image = input_image.getData();
		data.lambda = lambda;
		gc->setSmoothCost(&smoothFn, &data);
		gc->setVerbosity(verbosity);
		gc->swap(cycles);
		for (int i = 0; i < num_pixels; i++)
		{
			new_label_image->getData()[i] = label_array[gc->whatLabel(i)];
		}

		delete gc;
	}
	catch (GCException &e){
		e.Report();
	}
	return new_label_image;
}

int elib::smoothFn(int p1, int p2, int l1, int l2, void *data)
{
	ForSmoothFn fsf = *((ForSmoothFn*) data);
	if(l1==l2)
		return static_cast<int>(fsf.lambda*exp(-pow(fsf.image[p1]-fsf.image[p2],2)));
	else
	{
		if((l1==1 && l2!=0) || (l1!=0 && l2==1))
		{
			return GC_INFINITY;
		}
		else
		{
			return static_cast<float>((label_dist(l1-l2)) + fsf.lambda*exp(-pow(fsf.image[p1]-fsf.image[p2],2)));
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
