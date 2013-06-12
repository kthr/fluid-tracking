/*
 * main.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: kthierbach
 */

#include <algorithm>
#include "../lib/CImg.h"
#include "c_wrapper.h"
#include "connectedComponents.hpp"
#include "componentsMeasurements.hpp"

int main(int argc, char *argv[])
{
	cimg_library::CImg<int> image("/Users/kthierbach/Documents/current/emb/refdataA/bin/bin026.png");
	image = image.get_channel(0);
	int dimensions[2] = {image.width(), image.height()};
	cimage *bimage = createImage2(2, dimensions, 16, 1);
	std::copy(image.data(), image.data()+image.size(), bimage->data);

	cimage *label_image = elib::ConnectedComponents::getComponents(bimage);
	std::copy(label_image->data, label_image->data+label_image->flattened_length, image.data());
	//ComponentsMeasurements cm(label_image);
	image.save("/Users/kthierbach/label.png");

}
