/*
 * connectedComponents.hpp
 *
 *  Created on: Jun 10, 2013
 *      Author: kthierbach
 */

#ifndef CONNECTEDCOMPONENTS_HPP_
#define CONNECTEDCOMPONENTS_HPP_

class ConnectedComponents
{
	public:
		ConnectedComponents();
		virtual ~ConnectedComponents();

		const static short CONNECTIVITY_2D_4 = 0;
		const static short CONNECTIVITY_2D_8 = 1;
		static short connectivity_2d = CONNECTIVITY_2D_4;

		const static short CONNECTIVITY_3D_6 = 0;
		const static short CONNECTIVITY_3D_14 = 1;
		static short connectivity_3d = CONNECTIVITY_3D_6;

		static cimage* getComponents(cimage *image);
};

#endif /* CONNECTEDCOMPONENTS_HPP_ */
