/*
 * object.hpp
 *
 *  Created on: Jun 27, 2013
 *      Author: kthierbach
 */

#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <memory>
#include <libxml/xmlwriter.h>

#include "annotation.hpp"
#include "link.hpp"
#include "templates/mask.hpp"

namespace elib
{

class Link;

class Object
{
	public:
		Object();
		virtual ~Object();

		elib::Annotation* addAnnotation();
		elib::Link* addLink();
		void toXML(const xmlTextWriterPtr writer, bool compressed=true) const;

		int getFrameId() const;
		void setFrameId(int frameId);
		int getId() const;
		void setId(int id);
		const std::shared_ptr<Mask<glm::ivec2>> getMask() const;
		void setMask(std::shared_ptr<Mask<glm::ivec2>> &mask);
		int getTrackId() const;
		void setTrackId(int trackId);

	private:
		int id = -1,
			frameId = -1,
			trackId = -1;
		std::shared_ptr<Mask<glm::ivec2>> mask;
		std::vector<Link> links;
		std::vector<Annotation> annotations;

		void writePoint(const xmlTextWriterPtr writer, const glm::ivec2 &point) const;
};

} /* namespace elib */
#endif /* OBJECT_HPP_ */
