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

#include "object.hpp"

namespace elib
{

class Frame
{
	public:
		Frame();
		virtual ~Frame();
		Object* addObject(uint32_t trackId);
		const Object* getObject(uint32_t trackId) const;
		std::vector<Object>::iterator begin();
		std::vector<Object>::iterator end();
		void toXML(const xmlTextWriterPtr writer, bool compressed=true) const;
		uint32_t getNumObjects() const;
		const std::vector<Object>& getObjects() const;
		void setObjects(const std::vector<Object>& objects);
		bool isValid() const;
		void setValid(bool valid = true);

	private:
		bool valid = true;
		std::vector<Object> objects;
		std::unordered_map<uint32_t, uint32_t> track2objectId;
};

} /* namespace elib */
#endif /* FRAME_HPP_ */
