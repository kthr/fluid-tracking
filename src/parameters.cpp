/*
 * parameters.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "parameters.hpp"

#include <algorithm>

using elib::Parameters;

Parameters::Parameters(uint32_t int_params_size, int32_t *int_params, uint32_t double_params_size, double *double_params)
{
	this->int_params_size = int_params_size;
	this->int_params = new int32_t[int_params_size];
	this->double_params_size = double_params_size;
	this->double_params = new double[double_params_size];
	std::copy(int_params, int_params+int_params_size, this->int_params);
	std::copy(double_params, double_params+double_params_size, this->double_params);
}
Parameters::Parameters(parameters *params)
{
	this->int_params_size = (uint32_t)params->int_params_size;
	this->int_params = new int32_t[int_params_size];
	this->double_params_size = (uint32_t)params->double_params_size;
	this->double_params = new double[double_params_size];
	std::copy(params->int_params, params->int_params+this->int_params_size, this->int_params);
	std::copy(params->double_params, params->double_params+this->double_params_size, this->double_params);
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
