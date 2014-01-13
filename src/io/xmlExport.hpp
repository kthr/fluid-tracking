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

template<class T>
struct writeElementImpl
{
	static int writeElement(xmlTextWriterPtr writer, const std::string name, const T value)
	{
		std::stringstream tmp;
		tmp << value;
		return xmlTextWriterWriteAttribute(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
	}
};
template<>
struct writeElementImpl<std::string>
{
	static int writeElement(xmlTextWriterPtr writer, const std::string name, const std::string value)
	{
		std::stringstream tmp;
		tmp << value;
		return xmlTextWriterWriteAttribute(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
	}
};
template<>
struct writeElementImpl<const char *>
{
	static int writeElement(xmlTextWriterPtr writer, const std::string name, const char * value)
	{
		std::stringstream tmp;
		tmp << std::string(value);
		return xmlTextWriterWriteAttribute(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
	}
};

template <class T>
struct writeAttributeImpl
{
	static int writeAttribute(xmlTextWriterPtr writer, const std::string name, const T value)
	{
		std::stringstream tmp;
		tmp << std::to_string(value);
		return xmlTextWriterWriteAttribute(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
	}
};
template<>
struct writeAttributeImpl<std::string>
{
	static int writeAttribute(xmlTextWriterPtr writer, const std::string name, const std::string value)
	{
		std::stringstream tmp;
		tmp << value;
		return xmlTextWriterWriteAttribute(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
	}
};
template<>
struct writeAttributeImpl<const char *>
{
	static int writeAttribute(xmlTextWriterPtr writer, const std::string name, const char * value)
	{
		std::stringstream tmp;
		tmp << std::string(value);
		return xmlTextWriterWriteAttribute(writer, BAD_CAST name.c_str(), BAD_CAST tmp.str().c_str());
	}
};

class XMLExport
{
	public:
		XMLExport(const Parameters *params, const TrackingData *data);
		virtual ~XMLExport();

		void write(const char *uri);

		template <class T>
		static int writeElement(xmlTextWriterPtr writer, std::string name, T value)
		{
			return writeElementImpl<T>::writeElement(writer, name, value);
		}

		template <class T>
		static int writeAttribute(xmlTextWriterPtr writer, std::string name, T value)
		{
			return writeAttributeImpl<T>::writeAttribute(writer, name, value);
		}

	private:
		const Parameters *params;
		const TrackingData *data;
};

} /* namespace elib */
#endif /* XMLEXPORT_HPP_ */
