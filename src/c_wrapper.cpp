/*
 * c_wrapper.c
 *
 *  Created on: Jan 22, 2013
 *      Author: kthierbach
 */

#include <stdlib.h>

#include "c_wrapper.h"
#include "labeling.hpp"
#include "image.hpp"
#include "parameters.hpp"

void labeling_c(cimage *new_label_image, cimage* label_image, cimage* input_image, parameters *input_params)
{
	labeling(new_label_image, label_image, input_image, input_params);
}

cimage* cloneImage(cimage* image)
{
	cimage *new_image;

	new_image = (cimage*)malloc(sizeof(cimage));
	new_image->bit_depth = image->bit_depth;
	new_image->channels = image->channels;
	new_image->rank = image->rank;
	new_image->dimensions = (mint*)malloc(sizeof(mint)*image->rank);
	memcpy(new_image->dimensions, image->dimensions, sizeof(mint)*image->rank);
	new_image->flattened_length = image->flattened_length;
	new_image->data = (mint*)malloc(sizeof(mint)*new_image->flattened_length);
	memcpy(new_image->data, image->data, sizeof(mint)*new_image->flattened_length);
	new_image->shared = 0;

	return new_image;
}

cimage* createImage(cimage* image)
{
	cimage *new_image;

	new_image = (cimage*)malloc(sizeof(cimage));
	new_image->bit_depth = image->bit_depth;
	new_image->channels = image->channels;
	new_image->rank = image->rank;
	new_image->dimensions = (mint*)malloc(sizeof(mint)*image->rank);
	memcpy(new_image->dimensions, image->dimensions, sizeof(mint)*image->rank);
	new_image->flattened_length = image->flattened_length;
	new_image->data = (mint*)malloc(sizeof(mint)*new_image->flattened_length);
	new_image->shared = 0;

	return new_image;
}

cimage* createImage2(mint rank, mint *dimensions, mint bit_depth, mint channels)
{
	mint i;
	cimage *new_image;

	new_image = (cimage*)malloc(sizeof(cimage));
	new_image->bit_depth = bit_depth;
	new_image->channels = channels;
	new_image->rank = rank;
	new_image->dimensions = (mint*)malloc(sizeof(mint)*rank);
	memcpy(new_image->dimensions, dimensions, sizeof(mint)*rank);
	new_image->flattened_length = channels;
	for(i=0; i<new_image->rank; ++i)
	{
		new_image->flattened_length*=dimensions[i];
	}
	new_image->data = (mint*)malloc(sizeof(mint)*new_image->flattened_length);

	return new_image;
}

parameters* createParameters(mint int_params_size, mint double_params_size)
{
	parameters *params;

	params = (parameters*)malloc(sizeof(parameters));
	params->int_params_size = int_params_size;
	params->double_params_size = double_params_size;
	params->int_params = (mint*)malloc(sizeof(mint)*int_params_size);
	params->double_params = (double*)malloc(sizeof(double)*double_params_size);

	return params;
}

void freeImage(cimage* image)
{
	if(image->shared)
	{
		free(image);
	}
	else
	{
		free(image->data);
		free(image->dimensions);
		free(image);
	}
}

void freeParameters(parameters* params)
{
	free(params->int_params);
	free(params->double_params);
	free(params);
}
