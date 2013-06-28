/*
 * frame.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#ifndef FRAME_HPP_
#define FRAME_HPP_

#include <libxml/xmlwriter.h>
#include <stdint.h>
#include <unordered_map>
#include <vector>

namespace elib
{

class Object;

class Frame
{
	public:
		Frame();
		virtual ~Frame();
		Object* addObject(uint32_t trackId);
		Object* getObject(uint32_t trackId) const;
		std::vector<Object>::iterator begin() const;
		std::vector<Object>::iterator end() const;
		void toXML(const xmlTextWriterPtr writer) const;
		uint32_t getNumObjects() const;

	private:
		bool valid = true;
		std::vector<Object> objects;
		std::unordered_map<uint32_t, uint32_t> track2objectId;
};

} /* namespace elib */
#endif /* FRAME_HPP_ */
