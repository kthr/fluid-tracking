/*
 * main.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: kthierbach
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "alg/connectedComponents.hpp"
#include "alg/fluidTracks.hpp"
#include "io/xmlExport.hpp"
#include "templates/image.hpp"
#include "templates/maskList.hpp"
#include "utils/parameters.hpp"
#include "utils/rle.hpp"
#include "types.hpp"

int main(int argc, char *argv[])
{
	using elib::ConnectedComponents;
	using elib::FluidTracks;
	using elib::Image;
	using elib::Parameters;
	using elib::MaskList;
	using elib::RLE;
	using elib::XMLExport;

	std::string folder = "/Users/kthierbach/Documents/projects/eclipse/Labeling/tests/";
	int32_t int_params[1] = {0};
	double double_params[4] = {.6, .8, 1., 1.}; //c0, c1, lambda, mu
	std::vector<std::string> images({folder+"initial.png", folder+"smaller002.png", folder+"smaller003.png", folder+"smaller004.png", folder+"smaller005.png"});

	Parameters params = Parameters(1, int_params, 4, double_params);
	FluidTracks ft = FluidTracks(&images, &params);
	ft.setMinObjectSize(30);
	ft.track();
	std::vector<elib::MaskList2D> *frames = ft.getFrames();
	std::stringstream name;
	for(int i = 0; i<frames->size(); ++i)
	{
		name << "/Users/kthierbach/" << i << ".png";
		Image<int32_t> image =(*frames)[i].masksToImage(ft.getInitial()->getRank(), ft.getInitial()->getDimensions());
		Image<int32_t>::saveImage(name.str(), &image);
		name.str("");
	}

	XMLExport xmle;
	xmle.write("/Users/kthierbach/test.xml");

	std::unordered_map<int32_t, elib::Mask2D*>::iterator it;
	for(it=(*frames)[0].begin(); it!=(*frames)[0].end(); ++it)
	{
		std::string tmp = it->second->getBoxMask();
		std::cout << "Length:" << tmp.length() << std::endl;
		std::cout << tmp << std::endl;
		std::string encoded = RLE::encode(tmp);
		std::cout << "Length:" << encoded.length() << std::endl;
		std::cout << encoded << std::endl;
//		std::cout << RLE::decode(RLE::encode(tmp)) << std::endl;
	}
}
