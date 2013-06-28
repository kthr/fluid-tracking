/*
 * parameters.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <libxml/xmlwriter.h>
#include <stdint.h>
#include <string>
#include <vector>
#include "../c_wrapper.h"

namespace elib{

class Parameters
{
	public:
		Parameters();
		Parameters(uint32_t int_params_size, int32_t *int_params, std::vector<std::string> int_names, uint32_t double_params_size, double *double_params, std::vector<std::string> double_names);
		Parameters(parameters *param);
		virtual ~Parameters();
		int32_t getIntegerParam(uint32_t index) const;
		double getDoubleParam(uint32_t index) const;
		bool setDoubleParam(uint32_t index, double value);
		bool setIntParam(uint32_t index, int32_t value);
		void toXML(const xmlTextWriterPtr writer) const;

	private:
		uint32_t 	int_params_size = 0,
					double_params_size = 0;
		double *double_params;
		int32_t *int_params;
		std::vector<std::string> int_names, double_names;

		template <typename type>
		void writeParameters(const xmlTextWriterPtr writer, uint32_t size, std::vector<std::string> names, type *values) const;

};

} /* namespace elib */

#endif /* PARAMETERS_HPP_ */
