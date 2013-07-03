/*
 * link.cpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#include "link.hpp"

namespace elib
{

Link::Link()
{
	// TODO Auto-generated constructor stub

}

Link::~Link()
{
	// TODO Auto-generated destructor stub
}

const Object*& Link::getFrom()
{
	return from;
}

void Link::setFrom(const Object* from)
{
	this->from = from;
}

const Object*& Link::getTo()
{
	return to;
}

void Link::setTo(const Object* to)
{
	this->to = to;
}

uint32_t Link::getType() const
{
	return type;
}

void Link::toXML(const xmlTextWriterPtr writer) const
{
}

void Link::setType(uint32_t type)
{
	this->type = type;
}

} /* namespace elib */
