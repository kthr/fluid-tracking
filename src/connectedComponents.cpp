/*
 * connectedComponents.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "connectedComponents.hpp"

ConnectedComponents::ConnectedComponents()
{
	// TODO Auto-generated constructor stub

}

ConnectedComponents::~ConnectedComponents()
{
	// TODO Auto-generated destructor stub
}

cimage* getComponents(cimage *image)
{
	cimage *label_image;

	label_image = createImage(image);

	if(image->rank == 2) //2d-image
	{
		for(int i=0; i<image->dimensions[0]; ++i)
		{
			for(int j=0; j<image->dimensions[1]; ++j)
			{

			}
		}
	}
	else //3d-image
	{

	}

	return label_image;
}
