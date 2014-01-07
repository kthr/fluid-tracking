/*
 * parameters.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "parameters.hpp"

#include <limits>
#include <sstream>

namespace elib{

Parameters::Parameters()
{
}
Parameters::~Parameters()
{
}

bool Parameters::addParameter(std::string identifier, int value)
{
	integer_params.erase(identifier);
	auto res = integer_params.insert(std::pair<std::string,int>(identifier,value));
	return res.second;
}

bool Parameters::addParameter(std::string identifier, double value)
{
	double_params.erase(identifier);
	auto res = double_params.insert(std::pair<std::string,double>(identifier,value));
	return res.second;
}

bool Parameters::addParameter(std::string identifier, elib::Tensor<int> &value)
{
	integer_tensor_params.erase(identifier);
	auto res = integer_tensor_params.insert(std::pair<std::string, elib::Tensor<int> >(identifier, value));
	return res.second;
}

int Parameters::getIntegerParameter(std::string identifier) const
{
	auto res = integer_params.find(identifier);
	if(res == integer_params.end())
	{
		return std::numeric_limits<int>::quiet_NaN();
	}
	else
	{
		return res->second;
	}
}

double Parameters::getDoubleParameter(std::string identifier) const
{
	auto res = double_params.find(identifier);
	if(res == double_params.end())
	{
		return std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		return res->second;
	}
}

const elib::Tensor<int>* Parameters::getIntegerTensorParameter(std::string identifier) const
{
	auto res = integer_tensor_params.find(identifier);
	if(res == integer_tensor_params.end())
	{
		return nullptr;
	}
	else
	{
		return &(res->second);
	}
}

void Parameters::toXML(const xmlTextWriterPtr writer) const
{
	xmlTextWriterStartElement(writer, BAD_CAST "parameters"); /* start tracking parameters */
	writeParameters(writer, &integer_params);
	writeParameters(writer, &double_params);
	xmlTextWriterEndElement(writer); /* end tracking parameters */
}

template <typename type>
void Parameters::writeParameters(const xmlTextWriterPtr writer, const std::unordered_map<std::string, type> *params) const
{
	std::stringstream tmp;

	for (auto i = params->begin(); i != params->end(); ++i)
	{
		xmlTextWriterStartElement(writer, BAD_CAST "param");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST i->first.c_str());
		tmp << i->second;
		xmlTextWriterWriteAttribute(writer, BAD_CAST "value", BAD_CAST tmp.str().c_str());
		xmlTextWriterEndElement(writer); /* end param */
		tmp.str("");
	}
}

} /* end namespace elib */

