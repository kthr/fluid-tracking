/*
 * main.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: kthierbach
 */

#include "lib/CImg.h"
#include "c_wrapper.h"

int main(int argc, char *argv[])
{
	cimg::imagemagick_path("/opt/local/bin/convert");

	cimg_library::CImg<int> image("/Users/kthierbach/Documents/current/emb/refdataB/bin/bin275.png");
	mint dimensions[2] = {image.width(), image.height()};
	cimage *bimage = createImage2(2, dimensions, 8, 1);
	memcpy(bimage->data, image.data(), sizeof(mint)*bimage->flattened_length);

	cimage *label_image = ConnectedComponents::getComponents(bimage);
	memcpy(image.data(), label_image->data, sizeof(mint)*label_image->flattened_length);
	image.save("~/Users/kthierbach/label.png");

}
