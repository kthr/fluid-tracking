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
#include "templates/maskList.hpp"
#include "utils/parameters.hpp"

int main(int argc, char *argv[])
{
	using elib::ConnectedComponents;
	using elib::FluidTracks;
	using elib::Image;
	using elib::Parameters;
	using elib::MaskList;

	std::string folder = "/Users/kthierbach/Documents/projects/eclipse/Labeling/tests/";
	int32_t int_params[1] = {0};
	double double_params[4] = {.6, .8, 1., 1.}; //c0, c1, lambda, mu
	std::vector<std::string> images({folder+"initial.png", folder+"smaller002.png", folder+"smaller003.png", folder+"smaller004.png", folder+"smaller005.png"});

	Parameters params = Parameters(1, int_params, 4, double_params);
	FluidTracks ft = FluidTracks(&images, &params);
	ft.setMinObjectSize(30);
	ft.track();
	std::vector<MaskList<int32_t, glm::ivec3> > *frames = ft.getFrames();
	std::stringstream name;
	for(int i = 0; i<frames->size(); ++i)
	{
		name << "/Users/kthierbach/" << i << ".png";
		Image<int32_t> image =(*frames)[i].masksToImage(ft.getInitial()->getRank(), ft.getInitial()->getDimensions());
		Image<int32_t>::saveImage(name.str(), &image);
		name.str("");
	}
}
