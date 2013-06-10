/*
 * componentsMeasurements.cpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#include "componentsMeasurements.hpp"

ComponentsMeasurements::ComponentsMeasurements(cimage *label_image)
{
	this->label_image = label_image;
	init();
}

ComponentsMeasurements::~ComponentsMeasurements()
{
	// TODO Auto-generated destructor stub
}

void ComponentsMeasurements::init()
{
	labels.insert(label_image->data, label_image->data + label_image->flattened_length);

}
