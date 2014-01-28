/*
 * boundingBox.hpp
 *
 *  Created on: Jan 28, 2014
 *      Author: kthierbach
 */

#ifndef BOUNDINGBOX_HPP_
#define BOUNDINGBOX_HPP_

namespace elib
{

template <class Point>
class BoundingBox
{
	public:
		BoundingBox(Point upper_left, Point bottom_right) : upper_left(upper_left), bottom_right(bottom_right)
		{
		}
		virtual ~BoundingBox()
		{
		}

		bool inside(Point other)
		{
			if(upper_left <= other || other <= bottom_right)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		Point getUpperLeft() const
		{
			return upper_left;
		}
		Point getBottomRight() const
		{
			return bottom_right;
		}

	private:
		Point upper_left, bottom_right;
};

} /* namespace elib */

#endif /* BOUNDINGBOX_HPP_ */
