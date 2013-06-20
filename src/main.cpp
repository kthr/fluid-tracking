/*
 * main.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: kthierbach
 */

#include <string>
#include <vector>

#include "alg/connectedComponents.hpp"
#include "alg/fluidTracks.hpp"
#include "templates/image.hpp"
#include "utils/parameters.hpp"

int main(int argc, char *argv[])
{
	using elib::ConnectedComponents;
	using elib::FluidTracks;
	using elib::Image;
	using elib::Parameters;

	std::string folder = "/Users/kthierbach/Documents/projects/eclipse/Labeling/tests/";
	int32_t int_params[1] = {0};
	double double_params[4] = {.6, .8, 1., 1.}; //c0, c1, lambda, mu
	std::string bla[5] = {folder+"initial.png", folder+"smaller002.png", folder+"smaller003.png", folder+"smaller004.png", folder+"smaller005.png"};
	std::vector<std::string> images(bla, bla+3);

//	ConnectedComponents cc;
//	Image<int32_t> *test = Image<int32_t>::openImage(bla[0]);
//	test = cc.getComponents(test);
//	Image<int32_t>::saveImage("/Users/kthierbach/test.jpg",test);

	Parameters params = Parameters(1, int_params, 4, double_params);
	FluidTracks ft = FluidTracks(&images, &params);
	ft.track();
}
