/*
 * parameters.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <stdint.h>
#include <stdlib.h>

class Parameters
{
	public:
		Parameters(uint32_t int_params_size, int32_t *int_params, uint32_t double_params_size, double double_params);
		virtual ~Parameters();
		int32_t getIntegerParam(uint32_t index);
		double getDoubleParam(uint32_t index);
	private:
		uint32_t int_params_size, double_params_size;
		double *double_params;
		int32_t *int_params;
};

#endif /* PARAMETERS_HPP_ */
