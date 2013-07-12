/*
 * parameters.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "parameters.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>

namespace elib{

Parameters::Parameters()
{
	int_params = nullptr;
	double_params = nullptr;
	int_names = std::vector<std::string>();
	double_names = std::vector<std::string>();
}
Parameters::Parameters(uint32_t int_params_size, int32_t *int_params, std::vector<std::string> int_names, uint32_t double_params_size, double *double_params, std::vector<std::string> double_names)
: int_params_size(int_params_size), double_params_size(double_params_size)
{
	this->int_params = new int32_t[int_params_size];
	this->double_params = new double[double_params_size];
	this->int_names = std::vector<std::string>(int_params_size,"");
	this->double_names = std::vector<std::string>(double_params_size,"");

	std::copy(int_params, int_params+int_params_size, this->int_params);
	std::copy(double_params, double_params+double_params_size, this->double_params);
	std::copy(int_names.begin(), int_names.end(), this->int_names.begin());
	std::copy(double_names.begin(), double_names.end(), this->double_names.begin());
}
Parameters::Parameters(parameters *params)
{
	this->int_params_size = (uint32_t)params->int_params_size;
	this->int_params = new int32_t[int_params_size];
	this->double_params_size = (uint32_t)params->double_params_size;
	this->double_params = new double[double_params_size];
	this->int_names = std::vector<std::string>(int_params_size,"");
	this->double_names = std::vector<std::string>(double_params_size,"");

	std::copy(params->int_params, params->int_params+this->int_params_size, this->int_params);
	std::copy(params->double_params, params->double_params+this->double_params_size, this->double_params);
}

Parameters::~Parameters()
{
	delete[] int_params;
	delete[] double_params;
}

int32_t Parameters::getIntegerParam(uint32_t index) const
{
	return int_params[index];
}
double Parameters::getDoubleParam(uint32_t index) const
{
	return double_params[index];
}
bool Parameters::setDoubleParam(uint32_t index, double value)
{
	if(index >= double_params_size)
		return false;
	else
	{
		double_params[index] = value;
		return true;
	}
}
bool Parameters::setIntParam(uint32_t index, int32_t value)
{
	if(index >= int_params_size)
			return false;
	else
	{
		int_params[index] = value;
		return true;
	}
}
void Parameters::toXML(const xmlTextWriterPtr writer) const
{
	int rc;

	rc = xmlTextWriterStartElement(writer, BAD_CAST "parameters"); /* start tracking parameters */
	writeParameters(writer, int_params_size, int_names, int_params);
	writeParameters(writer, double_params_size, double_names, double_params);
	rc = xmlTextWriterEndElement(writer); /* end tracking parameters */
}
template <typename type>
void Parameters::writeParameters(const xmlTextWriterPtr writer, uint32_t size, std::vector<std::string> names, type *values) const
{
	int rc;
	std::stringstream tmp;

	for (uint32_t i = 0; i < size; ++i)
	{
		if (names[i].compare("") != 0)
		{
			rc = xmlTextWriterStartElement(writer, BAD_CAST "param");
			rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "name", BAD_CAST names[i].c_str());
			tmp << values[i];
			rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "value", BAD_CAST tmp.str().c_str());
			rc = xmlTextWriterEndElement(writer); /* end param */
			tmp.str("");
		}
	}
}

}

