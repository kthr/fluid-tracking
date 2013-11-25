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
		Object* addObject(int trackId);
		const Object* getObject(int trackId) const;
		std::vector<Object>::iterator begin();
		std::vector<Object>::iterator end();
		void toXML(const xmlTextWriterPtr writer, bool compressed=true) const;
		int getNumObjects() const;
		const std::vector<Object>& getObjects() const;
		void setObjects(const std::vector<Object>& objects);

	private:
		std::vector<Object> objects;
		std::unordered_map<int, int> track2objectId;
};

} /* namespace elib */
#endif /* FRAME_HPP_ */
