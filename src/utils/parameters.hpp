/*
 * parameters.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#ifndef PARAMETERS_HPP_
#define PARAMETERS_HPP_

#include <libxml/xmlwriter.h>
#include <set>
#include <string>
#include <unordered_map>

#include "templates/tensor.hpp"

namespace elib
{

class Parameters
{
	public:
		Parameters();
		virtual ~Parameters();
		bool addParameter(std::string identifier, int value);
		bool addParameter(std::string identifier, double value);
		bool addParameter(std::string identifier, elib::Tensor<int> &tensor);
		int getIntegerParameter(std::string identifier) const;
		double getDoubleParameter(std::string identifier) const;
		const elib::Tensor<int>* getIntegerTensorParameter(std::string identifier) const;
		void toXML(const xmlTextWriterPtr writer) const;

	private:
		std::unordered_map<std::string, int> integer_params;
		std::unordered_map<std::string, double> double_params;
		std::unordered_map<std::string, elib::Tensor<int> > integer_tensor_params;

		template <typename type>
		void writeParameters(const xmlTextWriterPtr writer, const std::unordered_map<std::string, type> *params) const;

};

} /* namespace elib */

#endif /* PARAMETERS_HPP_ */
