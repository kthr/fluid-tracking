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

namespace elib{

class Parameters
{
	public:
		Parameters(uint32_t int_params_size, int32_t *int_params, uint32_t double_params_size, double double_params);
		Parameters(parameters *param);
		virtual ~Parameters();
		int32_t getIntegerParam(uint32_t index);
		double getDoubleParam(uint32_t index);
	private:
		uint32_t int_params_size, double_params_size;
		double *double_params;
		int32_t *int_params;
};

} /* namespace elib */

#endif /* PARAMETERS_HPP_ */
