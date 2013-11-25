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

namespace elib{

class Parameters
{
	public:
		Parameters();
		Parameters(int int_params_size, int *int_params, std::vector<std::string> int_names, int double_params_size, double *double_params, std::vector<std::string> double_names);
		virtual ~Parameters();
		int getIntegerParam(int index) const;
		double getDoubleParam(int index) const;
		bool setDoubleParam(int index, double value);
		bool setIntParam(int index, int value);
		void toXML(const xmlTextWriterPtr writer) const;

	private:
		int 	int_params_size = 0,
					double_params_size = 0;
		double *double_params;
		int *int_params;
		std::vector<std::string> int_names, double_names;

		template <typename type>
		void writeParameters(const xmlTextWriterPtr writer, int size, std::vector<std::string> names, type *values) const;

};

} /* namespace elib */

#endif /* PARAMETERS_HPP_ */
