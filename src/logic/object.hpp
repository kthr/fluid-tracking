/*
 * object.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <libxml/xmlwriter.h>

#include "annotation.hpp"
#include "link.hpp"
#include "types.hpp"

namespace elib
{

class Object
{
	public:
		Object();
		virtual ~Object();

		void toXML(const xmlTextWriterPtr writer);

		uint32_t getFrameId() const;
		void setFrameId(uint32_t frameId);
		uint32_t getId() const;
		void setId(uint32_t id);
		const Mask2D*& getMask() const;
		void setMask(const Mask2D*& mask);
		uint32_t getTrackId() const;
		void setTrackId(uint32_t trackId);

	private:
		bool valid = true;
		uint32_t id, frameId, trackId;
		Mask2D *mask;
		std::vector<Link> links;
		std::vector<Annotation> annotations;
};

} /* namespace elib */
#endif /* OBJECT_HPP_ */
