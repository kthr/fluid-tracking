/*
 * link.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#ifndef LINK_HPP_
#define LINK_HPP_

#include <libxml/xmlwriter.h>
#include <stdint.h>
#include <vector>

#include "object.hpp"

namespace elib
{

class Object;

class Link
{
	public:
		Link();
		virtual ~Link();

		void toXML(const xmlTextWriterPtr writer) const;

		const Object*& getFrom();
		void setFrom(const Object* from);
		void setTo(const Object* to);
		int getType() const;
		void setType(int type);

		enum TYPE {SUCCESSOR = 0, DIVISION = 1};

	private:
		int type;
		const Object *from;
		std::vector<const Object*> to;
		double probability = 1.0;
};

} /* namespace elib */
#endif /* LINK_HPP_ */
