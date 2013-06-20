/*
 * MathematicaLibaryLink.c
 *
 *  Created on: Jan 22, 2013
 *      Author: kthierbach
 */

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "WolframLibrary.h"
#include "c_wrapper.h"

cimage* getImage(WolframLibraryData libData, MTensor* tensor, mint bit_depth, mint channels);

DLLEXPORT int llLabeling(WolframLibraryData libData, mint nargs, MArgument* input, MArgument output)
{
	cimage *label_image, *input_image, *new_label_image;
	parameters *params;
	MTensor new_label_tensor;

//	int debug=1;
//	while(debug);

	//get input
	label_image = getImage(libData, &MArgument_getMTensor(input[0]), 16, 1);
	input_image = getImage(libData, &MArgument_getMTensor(input[1]), MArgument_getInteger(input[2]), 1);
	params = createParameters(1, 4);
	params->int_params[0] = MArgument_getInteger(input[3]);// #labels
	params->double_params[0] = MArgument_getReal(input[4]);// c0
	params->double_params[1] = MArgument_getReal(input[5]);// c1
	params->double_params[2] = MArgument_getReal(input[6]);// lambda
	params->double_params[3] = MArgument_getReal(input[7]);// mu

	//compute cut
	new_label_image = labeling_c(label_image, input_image, params);

	//transform and write data to output
	libData->MTensor_new(MType_Integer,label_image->rank, label_image->dimensions, &new_label_tensor);
	memcpy(libData->MTensor_getIntegerData(new_label_tensor), new_label_image->data, sizeof(mint)*label_image->flattened_length);
	MArgument_setMTensor(output,new_label_tensor);

	freeImage(input_image);
	freeImage(label_image);
	freeImage(new_label_image);
	freeParameters(params);

	return LIBRARY_NO_ERROR;
}

cimage* getImage(WolframLibraryData libData, MTensor* tensor, mint bit_depth, mint channels)
{
	cimage *image;

	image = (cimage*)malloc(sizeof(cimage));
	image->bit_depth = bit_depth;
	image->channels = channels;
	if(channels==1)
	{
		image->rank = libData->MTensor_getRank(*tensor);
	}
	else
	{
		image->rank = libData->MTensor_getRank(*tensor)-1;
	}
	image->dimensions = libData->MTensor_getDimensions(*tensor);
	image->flattened_length = libData->MTensor_getFlattenedLength(*tensor);
	image->data = libData->MTensor_getIntegerData(*tensor);
	image->shared = 1;

	return image;
}

DLLEXPORT int WolframLibrary_getVersion(){return WolframLibraryVersion;}
DLLEXPORT int WolframLibrary_initialize( WolframLibraryData libData) {return 0;}
DLLEXPORT void WolframLibrary_uninitialize( WolframLibraryData libData) {}
