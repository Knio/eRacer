/**
 * @file AxisAlignedBoundingBox.cpp
 * @brief Implementation of the AxisAlignedBoundingBox class
 *
 * @date 10.01.2010
 * @author: Ole Rehmsen
 */

#include "AxisAlignedBoundingBox.h"
#include <cassert>

AxisAlignedBoundingBox::AxisAlignedBoundingBox()
	:	min_(ORIGIN),
	 	max_(ORIGIN)
{
}

AxisAlignedBoundingBox::~AxisAlignedBoundingBox() {
}


void AxisAlignedBoundingBox::resizeFromPoints(const std::vector<Point3>& points){
	if(points.empty()){
		reset();
		return;
	}

	min_ = max_ = points.front();

	for(vector<Point3>::const_iterator i = points.begin(); i!=points.end(); i++){
		if(i->x<min_.x)
			min_.x = i->x;
		else if(i->x>max_.x)
			max_.x = i->x;

		if(i->y<min_.y)
			min_.y = i->y;
		else if(i->y>max_.y)
			max_.y = i->y;

		if(i->z<min_.z)
			min_.z = i->z;
		else if(i->z>max_.z)
			max_.z = i->z;
	}
}

void AxisAlignedBoundingBox::reset(){
	min_ = max_ = ORIGIN;
}

void AxisAlignedBoundingBox::merge(const AxisAlignedBoundingBox& newBox){
	if(newBox.getMin().x<min_.x)
		min_.x = newBox.getMin().x;
	else if(newBox.getMax().x>max_.x)
		max_.x = newBox.getMax().x;

	if(newBox.getMin().y<min_.y)
		min_.y = newBox.getMin().y;
	else if(newBox.getMax().y>max_.y)
		max_.y = newBox.getMax().y;

	if(newBox.getMin().z<min_.z)
		min_.z = newBox.getMin().z;
	else if(newBox.getMax().z>max_.z)
		max_.z = newBox.getMax().z;
}

bool AxisAlignedBoundingBox::intersects(const Plane& plane) const{
	//TODO implement
}


Point3 AxisAlignedBoundingBox::getCorner(BoxCorners boxCorner) const{
	switch (boxCorner){
	case LEFT_BOTTOM_FRONT:
		return min_;
	case LEFT_BOTTOM_BACK:
		return Point3(min_.x, min_.y, max_.z);
	case LEFT_TOP_FRONT:
		return Point3(min_.x, max_.y, min_.z);
	case LEFT_TOP_BACK:
		return Point3(min_.x, max_.y, max_.z);
	case RIGHT_BOTTOM_FRONT:
		return Point3(max_.x, min_.y, min_.z);
	case RIGHT_BOTTOM_BACK:
		return Point3(max_.x, min_.y, max_.z);
	case RIGHT_TOP_FRONT:
		return Point3(max_.x, max_.y, min_.z);
	case RIGHT_TOP_BACK:
		return max_;
	}
}



