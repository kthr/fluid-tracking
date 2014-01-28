/*
 * GraphCutImage.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: kthierbach
 */

#include "graphcut.hpp"

#include "maxflow/energy.h"
#include "maxflow/graph.h"
#include "utils/math_functions.hpp"

namespace elib{

#define GC_INFINITY 300000

std::shared_ptr<Image<int>> graphcut(Image<int> &input_image, Parameters &parameters)
{
	std::shared_ptr<Image<int>> binary_image = std::shared_ptr<Image<int>>(new Image<int>(input_image.getRank(), *(input_image.getDimensions()), input_image.getBitDepth(), input_image.getChannels()));
	int nodeCount;
	int max_intensity, bg, fg, value;
	int x, y, z;
	int *nh,
		nh_length,
		nh2d[24] = {-1,0,0,-1,-1,0,0,-1,0,1,-1,0,1,0,0,1,1,0,0,1,0,-1,1,0}, // 8-neighborhood indices
		nh3d[42] = {-1,0,0,0,0,-1,1,0,0,0,0,1,0,1,0,0,-1,0,-1,1,-1,1,1,-1,-1,1,1,1,1,1,-1,-1,-1,1,-1,-1,-1,-1,1,1,-1,1};
	if(input_image.getRank()==2)
	{
		nh=nh2d;
		nh_length=24;
	}
	else
	{
		nh=nh3d;
		nh_length=42;
	}
	int width = input_image.getWidth(),
		height = input_image.getHeight(),
		depth = input_image.getDepth(),
		bit_depth = input_image.getBitDepth();

	double 	c0 = 0.,
			c1 = 1.,
			lambda1 = 0.,
			lambda2 = 1.;

	if(
		isnan(c0 = parameters.getDoubleParameter("C0")) ||
		isnan(c1 = parameters.getDoubleParameter("C1")) ||
		isnan(lambda1 = parameters.getDoubleParameter("Lambda")) ||
		isnan(lambda2 = parameters.getDoubleParameter("Mu"))
	)
	{
		return nullptr;
	}

	int *input_image_data = input_image.getData();
	int *binary_image_data = binary_image->getData();

	max_intensity = pow(2,bit_depth)-1;
	bg = c0*max_intensity;
	fg = c1*max_intensity;

	/****** Create the Energy *************************/
	int number_nodes = width*height*depth;
	gc::Energy energy;
	gc::Energy::Var varx[number_nodes];

	/****** Build Unary Term *************************/
	for(int k=0; k<depth; ++k )
	{
		for (int j=0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				nodeCount = i + j*width + k*width*height;

				// add node
				varx[nodeCount] = energy.add_variable();

				// add likelihood
				value = input_image_data[nodeCount];
				energy.add_term1(varx[nodeCount], abs(value - bg), abs(value - fg));
			}
		}
	}

	/******* Build pairwise terms ********************/
	for(int k=0; k<depth; ++k)
	{
		for (int j=0; j<height; ++j)
		{
			for (int i=0; i<width; ++i)
			{
				nodeCount = i + j*width + k*width*height;
				for(int l = 0; l < nh_length; l+=3)
				{
					x = i + nh[l];
					y = j + nh[l+1];
					z = k + nh[l+2];
					//if not outside of image
					if (!(x<0 || x>=width || y<0 || y>=height || z<0 || z>=depth))
					{
						value = lambda1
								+ lambda2
										* exp(pow(input_image_data[nodeCount] - input_image_data[x + y*width + z*width*height], 2));
						energy.add_term2(varx[nodeCount], varx[x + y*width + z*width*height], 0., value, value, 0.);
					}
				}
			}
		}
	}

	/******* Minimize energy ********************/
	energy.minimize();

	/******* Show binary image and clean up ********************/
	for(int k=0; k<depth; ++k)
	{
		for (int j = 0; j < height; ++j)
		{
			for (int i = 0; i < width; ++i)
			{
				nodeCount = i + j * width + k*width*height;
				if (energy.get_var(varx[nodeCount]))
				{
					binary_image_data[nodeCount] = 1;
				}
				else
				{
					binary_image_data[nodeCount] = 0;
				}
			}

		}
	}

	return binary_image;

}

} /* end namespace elib */
