/*
 * main.cpp
 *
 *  Created on: Jun 11, 2013
 *      Author: kthierbach
 */

#include "fluidTracking.hpp"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <getopt.h>
#include <iostream>
#include <string>

#include "alg/fluidTracks.hpp"
#include "CImg.h"
#include "exceptions/IOException.hpp"
#include "io/xmlExport.hpp"
#include "logic/trackingData.hpp"
#include "utils/parameters.hpp"
#include "utils/utilities.hpp"
#include "templates/image.hpp"
#include "types.hpp"

#include "utils/rle.hpp"

int main(int argc, char *argv[])
{
	cimg_library::cimg::exception_mode(0);

	using elib::FluidTracks;
	using elib::Image;
	using elib::IOException;
	using elib::Parameters;
	using elib::TrackingData;
	using elib::Utilities;
	using elib::XMLExport;
	namespace fs = boost::filesystem;
	namespace po = boost::program_options;

	int32_t min = 0, max = INT32_MAX;
	double c0 = .1, c1 = .9, mu = 1., lambda = 1.;
	std::string initial = "", image_folder, vector_field_folder, label_image_folder = "", in;
	int iao = 1, verbose = 0;
	bool compressed = false;
	int option, option_index;
	std::vector<std::string> images, vector_fields;
	bool without_flows = false;

//	po::options_description desc("Allowed options");
//	desc.add_options()
//		("c0", po::value<double>(&c0)->default_value(0.1), "mean background intensity (between 0. and 1.)")
//		("c1", po::value<double>(&c1)->default_value(0.9), "mean foreground intensity (between 0. and 1.)")
//		("help,h", "produce help message")
//		("i", po::value<std::string>(&initial)->default_value(""), "initial label image")
//		("include-appearing-objects,o", po::value<bool>(&iao)->default_value(true), "include appearing objects")
//		("lambda,l", po::value<double>(&lambda)->default_value(1.), "parameter lambda")
//		("max", po::value<int>(&max)->default_value(INT32_MAX), "maximal object size in pixels")
//		("min", po::value<int>(&min)->default_value(0), "minimal object size in pixels")
//		("mu,m", po::value<double>(&mu)->default_value(1.), "parameter mu")
//		("write-label-images,w", po::value<std::string>(&label_image_folder), "write label images to the given directory")
//	;
//	po::positional_options_description pd;
//	pd.add("images", 1).add("vector_fields", 1);
//
//	po::variables_map vm;
//	po::store(po::command_line_parser(argc, argv).options(desc).positional(pd).run(), vm);
//	po::notify(vm);
//
//	if (vm.count("help")) {
//		std::cout << desc << "\n";
//		return EXIT_SUCCESS;
//	}
//
//	if (vm.count("images")) //TODO print usage message if not found
//	{
//		image_folder = vm["images"].as<std::string>();
//	}
//	if (vm.count("vector_fields"))
//	{
//		vector_field_folder = vm["vector_fields"].as<std::string>();
//	}

	struct option long_options[] =
	{
	{ "min", required_argument, NULL, '1' },
	{ "max", required_argument, NULL, '2' },
	{ "c0", required_argument, NULL, '3' },
	{ "c1", required_argument, NULL, '4' },
	{ "write-label-images", required_argument, NULL, '5' },
	{ "include-appearing-objects", no_argument, &iao, 1 },
	{ "verbose", no_argument, &verbose, 1 },
	{ 0, 0, 0, 0 } };
	while (1)
	{
		option = getopt_long(argc, argv, "chm:l:i:ow:", long_options, &option_index);
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
				label_image_folder = std::string(optarg);
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
				label_image_folder = std::string(optarg);
				break;
			case 'c': // initial
				compressed = true;
				break;
			case 'h':
#ifdef REVISION
				std::cout << "Revision: " << REVISION << std::endl;
#else
				std::cout << "Revision: Unknown" << std::endl;
#endif
				std::cout << "\t --c0 NUM" << std::endl << "\t\t mean background intensity (default=.1)" << std::endl;
				std::cout << "\t --c1 NUM" << std::endl << "\t\t mean foreground intensity (default=.9)" << std::endl;
				std::cout << "\t --max NUM" << std::endl << "\t\t maximal object size in pixels  (default=" << INT32_MAX
						<< ")" << std::endl;
				std::cout << "\t --min NUM" << std::endl << "\t\t minimal object size in pixels (default=0)"
						<< std::endl;
				std::cout << "\t -c" << std::endl << "\t\t turn on compression" << std::endl;
				std::cout << "\t -i IMAGE" << std::endl << "\t\t initial label image" << std::endl;
				std::cout << "\t -l NUM" << std::endl << "\t\t parameter lambda (default=1.)" << std::endl;
				std::cout << "\t -m NUM" << std::endl << "\t\t parameter mu (default=1.)" << std::endl;
				std::cout << "\t -o, --include-appearing-objects" << std::endl
						<< "\t\t include appearing objects (default=true)" << std::endl;
				std::cout << "\t -w, --write-label-images DIR" << std::endl
						<< "\t\t write label images to the given directory" << std::endl;
				std::cout << "Example: fluidTracking --c0 .6 --c1 .9 --iao --min 25 images/ vector_fields/"
						<< std::endl;
				std::cout << "Tracking parameters:" << std::endl;
				std::cout << "Usage: fluidTracking [OPTIONS]... IMAGE_FOLDER [VECTOR_FIELD_FOLDER]" << std::endl;
				return EXIT_SUCCESS;
			case '?':
				/* getopt_long already printed an error message. */
				break;
			default:
				abort();
		}
	}
	if ((argc - optind) < 1)
	{
		std::cerr << "Usage: fluidTracking [OPTIONS]... IMAGE_FOLDER [VECTOR_FIELD_FOLDER]" << std::endl;
		std::cerr << "Try `fluidTracking -h' for more information." << std::endl;
		return EXIT_FAILURE;
	}
	if (argc - optind == 1)
	{
		image_folder = std::string(argv[optind]);
		without_flows = true;
	}
	else
	{
		image_folder = std::string(argv[optind++]);
		vector_field_folder = std::string(argv[optind]);
	}

	images =
			Utilities::getFiles(image_folder,
					boost::regex(
							"(.*\\.png$)|(.*\\.jpg$)|(.*\\.jpeg$)|(.*\\.tif$)|(.*\\.tiff$)|(.*\\.PNG$)|(.*\\.JPG$)|(.*\\.JPEG$)|(.*\\.TIF$)|(.*\\.TIFF$)"));
	vector_fields = Utilities::getFiles(image_folder, boost::regex("(.*\\.dat$)"));
	if (images.size() == 0)
	{
		std::cerr << "ERROR: No images found!" << std::endl;
		return EXIT_FAILURE;
	}

	std::cout << "Found " << images.size() << " images." << std::endl;
	if (without_flows)
	{
		std::cout << "Tracking without label displacement!" << std::endl;
	}
	else
	{
		std::cout << "Found " << vector_fields.size() << " vector fields." << std::endl;
	}

	if (label_image_folder.compare("") != 0)
	{
		if (fs::exists(label_image_folder) && fs::is_directory(label_image_folder) && !fs::is_empty(label_image_folder)) //TODO fix this is probably not working on other OS's
		{
			std::cout << "WARNING: Label directory '" << label_image_folder
					<< "' is not empty, files may be overwritten! Continue anyway? (y/N)";
			std::getline(std::cin, in);
			if (in.compare("y") != 0)
				return EXIT_FAILURE;
		}
	}

	int32_t int_params[1] = { 0 };
	double double_params[4] = { c0, c1, lambda, mu };
	std::vector<std::string> int_names({ "" });
	std::vector<std::string> double_names({ "C0", "C1", "Lambda", "Mu" });
	Parameters params(1, int_params, int_names, 4, double_params, double_names);

	FluidTracks ft(&params, &images, &vector_fields);
	ft.setMinObjectSize(min);
	ft.setMaxObjectSize(max);
	ft.setInitialMaskImage(initial);
	ft.setIncludeAppearing(iao);

	try
	{
		ft.track();
	} catch (const char* e)
	{
		std::cerr << e << std::endl;
		return EXIT_FAILURE;
	}

	TrackingData td(&ft, compressed);
	td.construct();

	XMLExport xe(&params, &td);
	std::string output_file = std::string("./") + Utilities::getTime() + std::string("-data-frame.xml");
	xe.write(output_file.c_str());

	if (label_image_folder.compare("") != 0)
	{
		std::vector<elib::MaskList2D>::iterator it;
		std::string file_name;
		Image<int32_t> image;
		int i = 0;
		for (it = ft.getFrames()->begin(); it != ft.getFrames()->end(); ++it)
		{
			file_name = Utilities::createFileName(label_image_folder, std::string("label"), std::string(".png"), i);
			image = it->masksToImage(ft.getInitial()->getRank(), ft.getInitial()->getDimensions());
			Image<int32_t>::saveImage(file_name, &image);
			++i;
		}
	}

	return EXIT_SUCCESS;
}
