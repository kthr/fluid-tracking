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
#include "templates/image.hpp"

using elib::Image;

int main(int argc, char *argv[])
{
	cimg_library::CImg<int32_t> image("/Users/kthierbach/Documents/current/emb/refdataB/bin/bin045.png");
	image = image.get_channel(0);
	uint32_t dimensions[2] = {image.width(), image.height()};
	elib::Image<int32_t> *bimage = new elib::Image<int32_t>(2, dimensions, 16, 1);
	bimage->setData(image.data());

	Image<int32_t> *label_image = elib::ConnectedComponents::getComponents(bimage);
	std::copy(label_image->getData(), label_image->getData()+label_image->getFlattenedLength(), image.data());
//	ComponentsMeasurements cm(label_image);
	image.save("/Users/kthierbach/label.png");
	delete bimage;
}
