/**
 * @file AxisAlignedBoundingBox.h
 * @brief Definition of the AxisAlignedBoundingBox class
 *
 * @date 10.01.2010
 * @author: Ole Rehmsen
 */

#ifndef AXISALIGNEDBOUNDINGBOX_H_
#define AXISALIGNEDBOUNDINGBOX_H_

#include "../Core/types.h"
#include <vector>

using namespace std;

/**
 * @brief Enum to reference the 8 corners of a box
 */
enum BoxCorners {
	LEFT_BOTTOM_FRONT,
	LEFT_BOTTOM_BACK,
	LEFT_TOP_FRONT,
	LEFT_TOP_BACK,
	RIGHT_BOTTOM_FRONT,
	RIGHT_BOTTOM_BACK,
	RIGHT_TOP_FRONT,
	RIGHT_TOP_BACK
};

/**
 * @brief Bounding box whose sides are parallel to the axes of the coordinate system.
 */
class AxisAlignedBoundingBox {
public:
	/**
	 * @brief Constructor. Create bounding box at origin with zero extent.
	 */
	AxisAlignedBoundingBox();
	
	/**
	 * @brief Destructor stub.
	 */
	~AxisAlignedBoundingBox();

	/**
	 * @brief Resize so that all points fit in the AABB
	 *
	 * @param points 
	 *		A vector of points that should be contained in the bounding volume. 
	 *		If empty, the bounding box will have zero extent.
	 */
	void resizeFromPoints(const vector<Point3>& points);
	
	/**
	 * @brief Set the bounding volume to have zero extent.
	 */
	void reset();

	/**
	 * @brief Increase the the size of this bounding volume until it also contains newBox.
	 * 
	 * @param newBox Another AABB that should be merged with this one.
	 */
	void merge(const AxisAlignedBoundingBox& newBox);


	bool intersects(const Plane& plane) const;

	/**
	 * @return The center of the box
	 */
	Point3 getCenter() const;
	
	/**
	 * @brief Getter for the 8 corners of the box.
	 * @param boxCorner
	 *			the corner you want to get
	 * @return the point in 3-D space where the corner is located
	 * @see BoxCorner
	 */
	Point3 getCorner(BoxCorners boxCorner) const;

	/**
	 * @brief getter for the left bottom front corner
	 *
	 * This is equivalent to calling getCorner(LEFT_BOTTOM_FRONT), 
	 * but does not require to copy the point.
	 *
	 * @return a constant reference to the left bottom front corner of the box
	 */
	const Point3& getMin() const;

	/**
	 * @brief getter for the right top back corner
	 *
	 * This is equivalent to calling getCorner(RIGHT_TOP_BACK), 
	 * but does not require to copy the point.
	 *
	 * @return a constant reference to the right top back corner of the box
	 */
	const Point3& getMax() const;

private:
	Point3 min_;
	Point3 max_;
};

inline Point3 AxisAlignedBoundingBox::getCenter() const { 
	return .5 * (min_+max_); 
}

inline const Point3& AxisAlignedBoundingBox::getMin() const { 
	return min_; 
}
inline const Point3& AxisAlignedBoundingBox::getMax() const { 
	return max_; 
}


#endif /* AXISALIGNEDBOUNDINGBOX_H_ */
