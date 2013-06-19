/*
 * c_wrapper.h
 *
 *  Created on: Jan 22, 2013
 *      Author: kthierbach
 */

#ifndef C_WRAPPER_H_
#define C_WRAPPER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "WolframLibrary.h"

typedef struct {
	mint *data;
	mint *dimensions;
	mint bit_depth;
	mint channels;
	mint flattened_length;
	mint rank;
	short shared;
} cimage;

typedef struct {
	double *double_params;
	mint *int_params;
	mint double_params_size;
	mint int_params_size;
} parameters;

/* Write C function declarations here */
	cimage* labeling_c(cimage* label_image, cimage* input_image, parameters *input_params);

	cimage* cloneImage(cimage *image);
	cimage* createImage(cimage *image);
	cimage* createImage2(mint rank, mint *dimensions, mint bit_depth, mint channels);
	parameters* createParameters(mint int_params_size, mint double_params_size);
	void freeImage(cimage *image);
	void freeParameters(parameters *param);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* C_WRAPPER_H_ */
