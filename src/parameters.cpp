/*
 * parameters.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "parameters.hpp"

Parameters::Parameters(uint32_t int_params_size, int32_t *int_params, uint32_t double_params_size, double double_params)
{
	this->int_params_size = int_params_size;
	this->int_params = new int32_t[int_params_size];
	this->double_params_size = double_params_size;
	this->double_params = new double[double_params_size];
	memcpy(this->int_params, int_params, sizeof(int32_t)*int_params_size);
	memcpy(this->double_params, double_params, sizeof(int32_t)*double_params_size);
}

Parameters::~Parameters()
{
	delete int_params;
	delete double_params;
}

int32_t Parameters::getIntegerParam(uint32_t index)
{
	return int_params[index];
}

double Parameters::getDoubleParam(uint32_t index)
{
	return double_params[index];
}
