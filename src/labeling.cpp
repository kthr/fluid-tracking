/*
 * GraphCutImage.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: kthierbach
 */

#include "labeling.hpp"
#include <tr1/unordered_map>
#include <set>

#define GC_INFINITY 300000

using elib::Labeling;

void labeling(cimage *new_label_image, cimage* label_image, cimage* input_image, parameters *input_params)
{
	int width, height, bit_depth, num_labels;
	double c0, c1, lambda, mu;

	width = label_image->dimensions[0];
	height = label_image->dimensions[1];
	bit_depth = input_image->bit_depth;
	num_labels = input_params->int_params[0];
	c0 = input_params->double_params[0];
	c1 = input_params->double_params[1];
	lambda = input_params->double_params[2];
	mu = input_params->double_params[3];
	std::set<int> labels;
	std::set<int>::iterator it;
	std::tr1::unordered_map<int,int> label_map;
	int *label_array, label;

	labels.insert(label_image->data, label_image->data+label_image->flattened_length);
	labels.insert(1); //inserts label 1
	label_array = new int[labels.size()];
	std::copy(labels.begin(), labels.end(), label_array);

	for(int i=0; i<labels.size(); ++i)
	{
		label_map.insert(std::pair<int,int>(label_array[i],i));
	}

	try{
		GCoptimizationGridGraph *gc = new GCoptimizationGridGraph(width,height, num_labels);
		double c;
		int numPixels = width*height;
		// first set up data costs individually

		c = c1*(pow(2,bit_depth)-1);
		//label for appearing objects == 1
		for ( int i = 0; i < numPixels; i++ )
		{
			label = label_map.find((int)label_image->data[i])->second;
			gc->setDataCost(i,1, int(2*mu*(label_dist(0-label))+fabs((double)input_image->data[i]-c)));
		}
		for (int l = 2; l < num_labels; l++ )
		{
			for ( int i = 0; i < numPixels; i++ )
			{
				label = label_map.find((int)label_image->data[i])->second;
				gc->setDataCost(i,l, int(mu*(label_dist(l-label))+fabs((double)input_image->data[i]-c)));
			}
		}
		//background labeling == 0
		c = c0*(pow(2,bit_depth)-1);
		for ( int i = 0; i < numPixels; i++ )
		{
			label = label_map.find((int)label_image->data[i])->second;
			gc->setDataCost(i,0, int(mu*(label_dist(0-label)))+fabs((double)input_image->data[i]-c));
		}

//		 //next set up smoothness costs individually
//				for ( int l1 = 0; l1 < numLabels; l1++ )
//					for (int l2 = 0; l2 < numLabels; l2++ )
//					{
//						gc->setSmoothCost(l1,l2,l1==l2 ? 0 : lambda);
//					}
		ForSmoothFn data;
		data.image = input_image->data;
		data.lambda = lambda;
		gc->setSmoothCost(&smoothFn, &data);

		gc->swap();
		for ( int  i = 0; i < numPixels; i++)
			new_label_image->data[i] = label_array[gc->whatLabel(i)];

		delete gc;
	}
	catch (GCException &e){
		e.Report();
	}

}

int smoothFn(int p1, int p2, int l1, int l2, void *data)
{
	ForSmoothFn fsf = *((ForSmoothFn*) data);
	if(l1==l2)
		return 0;
	else
		if((l1==1 && l2!=0) || (l1!=0 && l2==1))
			return GC_INFINITY;
		else
			return int(fsf.lambda*(label_dist(l1-l2)) + exp(-pow(fsf.image[p1]-fsf.image[p2],2)));
}

int label_dist(int value)
{
	if(value==0)
		return 0;
	else
		return 1;
}
