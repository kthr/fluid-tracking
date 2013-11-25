/*
 * annotation.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#ifndef ANNOTATION_HPP_
#define ANNOTATION_HPP_

#include <libxml/xmlwriter.h>
#include <string>

namespace elib
{

class Annotation
{
	public:
		Annotation();
		virtual ~Annotation();
		void toXML(const xmlTextWriterPtr writer) const;

	private:
//		bool valid = true;
		std::string type = "";
		std::string value = "";
};

} /* namespace elib */
#endif /* ANNOTATION_HPP_ */
