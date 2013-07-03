/*
 * main.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: kthierbach
 */

#include "fluidTracking.hpp"

#include <dirent.h>
#include <getopt.h>
#include <iostream>
#include <regex>
#include <string>

#include "alg/fluidTracks.hpp"
#include "CImg.h"
#include "exceptions/IOException.hpp"
#include "io/xmlExport.hpp"
#include "logic/trackingData.hpp"
#include "utils/parameters.hpp"

//int main(int argc, char *argv[])
//{
//	using elib::ConnectedComponents;
//	using elib::FluidTracks;
//	using elib::Image;
//	using elib::MaskList;
//	using elib::Parameters;
//	using elib::RLE;
//	using elib::TrackingData;
//	using elib::XMLExport;
//
//	std::string folder = "/Users/kthierbach/Documents/projects/eclipse/Labeling/tests/";
//	int32_t int_params[1] = {0};
//	double double_params[4] = {.6, .8, 1., 1.}; //c0, c1, lambda, mu
//	std::vector<std::string> int_names({""});
//	std::vector<std::string> double_names({"C0", "C1", "Lambda", "Mu"});
//	std::vector<std::string> images({folder+"initial.png", folder+"smaller002.png", folder+"smaller003.png", folder+"smaller004.png", folder+"smaller005.png"});
//
//	Parameters params = Parameters(1, int_params, int_names, 4, double_params, double_names);
//	FluidTracks ft = FluidTracks(&images, &params);
//	ft.setMinObjectSize(30);
//	ft.track();
//	std::vector<elib::MaskList2D> *frames = ft.getFrames();
//	std::stringstream name;
//	for(int i = 0; i<frames->size(); ++i)
//	{
//		name << "/Users/kthierbach/" << i << ".png";
//		Image<int32_t> image =(*frames)[i].masksToImage(ft.getInitial()->getRank(), ft.getInitial()->getDimensions());
//		Image<int32_t>::saveImage(name.str(), &image);
//		name.str("");
//	}
//	TrackingData td(ft.getFrames());
//	td.construct();
//	XMLExport xmle(&params, &td);
//	xmle.write("/Users/kthierbach/test.xml");
//
//	std::unordered_map<int32_t, elib::Mask2D*>::iterator it;
//	for(it=(*frames)[0].begin(); it!=(*frames)[0].end(); ++it)
//	{
//		std::string tmp = it->second->getBoxMask();
//		std::cout << "Length:" << tmp.length() << std::endl;
//		std::cout << tmp << std::endl;
//		std::string encoded = RLE::encode(tmp);
//		std::cout << "Length:" << encoded.length() << std::endl;
//		std::cout << encoded << std::endl;
//	}
//}

std::vector<std::string> getFiles(std::string directory, std::regex regex);

int main(int argc, char *argv[])
{
	cimg_library::cimg::exception_mode(0);

	using elib::FluidTracks;
	using elib::IOException;
	using elib::Parameters;
	using elib::TrackingData;
	using elib::XMLExport;

	int32_t min = 0,
			max = INT32_MAX;
	double  c0 = .1,
			c1 = .9,
			mu = 1.,
			lambda = 1.;
	std::string initial = "",
				image_folder,
				vector_field_folder,
				label_image_directory = "",
				in;
	int iao = false,
		verbose = false;
	int option, option_index;
	std::vector<std::string> images,
							 vector_fields;
	bool without_flows = false;

	struct option long_options[] =
	{
		{ "min", required_argument, NULL, '1' },
		{ "max", required_argument, NULL, '2' },
		{ "c0", required_argument, NULL, '3' },
		{ "c1", required_argument, NULL, '4' },
		{ "write-label-images", required_argument, NULL, '5' },
		{ "include-appearing-objects", no_argument, &iao, 0 },
		{ "verbose", no_argument, &verbose, 1 },
		{ 0, 0, 0, 0 }
	};

	while (1)
	{
		option = getopt_long(argc, argv, "hm:l:i:ow:", long_options, &option_index);
		if (option == -1)
		{
			break;
		}
		switch (option)
		{
			case 0: //flag option
				/*option sets a flag*/
				break;
			case '1': // min
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				min = atoi(optarg);
				break;
			case '2': // max
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				max = atoi(optarg);
				break;
			case '3': // c0
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				c0 = atof(optarg);
				break;
			case '4': // c1
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				c1 = atof(optarg);
				break;
			case '5': // label_image_directory
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				label_image_directory = std::string(optarg);
				break;
			case 'm': // mu
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				mu = atof(optarg);
				break;
			case 'l': // lambda
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				lambda = atof(optarg);
				break;
			case 'i': // initial
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				initial = std::string(optarg);
				break;
			case 'o': // initial
				iao = true;
				break;
			case 'w': // initial
				if (optarg == NULL)
				{
					fprintf(stderr, "Missing argument for option -%c.\n", option);
					return EXIT_FAILURE;
				}
				label_image_directory = std::string(optarg);
				break;
			case 'h':
#ifdef REVISION
				std::cout << "Revision: " << REVISION << std::endl;
#else
				std::cout << "Revision: Unknown" << std::endl;
#endif
				std::cout << "Usage: fluidTracking [OPTIONS]... IMAGE_FOLDER [VECTOR_FIELD_FOLDER]" << std::endl;
				std::cout << "Example: fluidTracking --c0 .6 --c1 .9 --iao --min 25 images/ vector_fields/" << std::endl;
				std::cout << "Tracking parameters:" << std::endl;
				std::cout << "\t --c0 NUM" << std::endl << "\t\t mean background intensity (default=.1)" << std::endl;
				std::cout << "\t --c1 NUM" << std::endl << "\t\t mean foreground intensity (default=.9)" << std::endl;
				std::cout << "\t -o, --include-appearing-objects" << std::endl << "\t\t include appearing objects (default=true)" << std::endl;
				std::cout << "\t --max NUM" << std::endl << "\t\t maximal object size in pixels  (default=" << INT32_MAX << ")" << std::endl;
				std::cout << "\t --min NUM" << std::endl << "\t\t minimal object size in pixels (default=0)" << std::endl;
				std::cout << "\t -w, --write-label-images DIR" << std::endl << "\t\t write label images to the given directory" << std::endl;
				std::cout << "\t -i IMAGE" << std::endl << "\t\t initial label image" << std::endl;
				std::cout << "\t -l NUM" << std::endl << "\t\t parameter lambda (default=1.)" << std::endl;
				std::cout << "\t -m NUM" << std::endl << "\t\t parameter mu (default=1.)" << std::endl;
				return EXIT_SUCCESS;
			case '?':
				/* getopt_long already printed an error message. */
				break;
			default:
				abort();
		}
	}
	if ((argc-optind) < 1)
	{
		std::cerr << "Usage: fluidTracking [OPTIONS]... IMAGE_FOLDER [VECTOR_FIELD_FOLDER]" << std::endl;
		std::cerr << "Try `fluidTracking -h' for more information." << std::endl;
		return EXIT_FAILURE;
	}
	if(argc-optind == 1)
	{
		image_folder = std::string(argv[optind]);
		without_flows=true;
	}
	else
	{
		image_folder = std::string(argv[optind++]);
		vector_field_folder = std::string(argv[optind]);
	}
	images = getFiles(image_folder, std::regex("(.*\\.png$)|(.*\\.jpg$)|(.*\\.jpeg$)|(.*\\.tif$)|(.*\\.tiff$)|(.*\\.PNG$)|(.*\\.JPG$)|(.*\\.JPEG$)|(.*\\.TIF$)|(.*\\.TIFF$)"));
	vector_fields = getFiles(image_folder, std::regex("(.*\\.dat$)"));
	if(images.size() == 0)
	{
		std::cerr << "ERROR: No images found!" << std::endl;
		return EXIT_FAILURE;
	}

//	for(int i=0; i<images.size(); ++i)
//	{
//		std::cout << images[i] << std::endl;
//	}

	std::cout << "Found " << images.size() << " images." << std::endl;
	if(without_flows)
	{
		std::cout << "Tracking without label displacement!" << std::endl;
	}
	else
	{
		std::cout << "Found " << vector_fields.size() << " vector fields." << std::endl;
	}


	std::cout << c0 << " " << iao << " " << label_image_directory << std::endl;
	if(label_image_directory.compare("") != 0)
	{
		if(getFiles(label_image_directory, std::regex(".*")).size() != 0)
		{
			std::cout << "WARNING: Label directory '" << label_image_directory << "' is not empty, files may be overwritten! Continue anyway? (y/n)";
			std::cin >> in;
			if(in.compare("y") != 0)
				return EXIT_FAILURE;
		}
	}

	int32_t int_params[1] = {0};
	double double_params[4] = {c0, c1, lambda, mu};
	std::vector<std::string> int_names({""});
	std::vector<std::string> double_names({"C0", "C1", "Lambda", "Mu"});
	Parameters params(1, int_params, int_names, 4, double_params, double_names);

	FluidTracks ft(&params, &images, &vector_fields);
	ft.setMinObjectSize(min);
	ft.setMaxObjectSize(max);
	ft.setInitialMaskImage(initial);
	ft.setIncludeAppearing(iao);

	try{
		ft.track();
	}
	catch(const char* e)
	{
		std::cerr << e << std::endl;
		return EXIT_FAILURE;
	}

	TrackingData td(ft.getFrames());
	td.construct();

	XMLExport xe(&params, &td);
	std::string output_file = std::string("./") + xe.getTime() + std::string("-data-frame.xml");
	xe.write(output_file.c_str());

	return EXIT_SUCCESS;
}

std::vector<std::string> getFiles(std::string directory, std::regex regex)
{
	DIR *dir = nullptr;
	struct dirent *drnt = nullptr;
	std::string file;
	std::vector<std::string> files;

	if(directory.compare("") != 0)
	{
		dir = opendir(directory.c_str());
		if(dir != nullptr)
		{
			while((drnt = readdir(dir)) != nullptr)
			{
				file = std::string(drnt->d_name);
				if(std::regex_match(file, regex))
					files.push_back(directory + file);
			}
			closedir(dir);
		}
		else
		{
			std::cerr << "ERROR: Directory " << directory << " not found!" << std::endl;
			abort();
		}
	}
	return files;
}
