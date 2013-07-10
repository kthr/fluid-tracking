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
		const Object*& getTo();
		void setTo(const Object* to);
		uint32_t getType() const;
		void setType(uint32_t type);

		enum TYPE {PREDECCESSOR = 0, SUCCESSOR = 1, DIVISION = 2};

	private:
		bool valid = true;
		uint32_t type;
		const Object *from, *to;
		double probability = 1.;
};

} /* namespace elib */
#endif /* LINK_HPP_ */
