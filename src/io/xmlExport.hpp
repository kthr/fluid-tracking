/*
 * xmlExport.hpp
 *
 *  Created on: Jun 24, 2013
 *      Author: kthierbach
 */

#ifndef XMLEXPORT_HPP_
#define XMLEXPORT_HPP_

#include <string>

#include "utils/parameters.hpp"
#include "logic/trackingData.hpp"

namespace elib
{

class XMLExport
{
	public:
		XMLExport(const Parameters *params, const TrackingData *data);
		virtual ~XMLExport();

		void write(const char *uri);
	private:
		const Parameters *params;
		const TrackingData *data;
};

} /* namespace elib */
#endif /* XMLEXPORT_HPP_ */
