/**
 * @file AxisAlignedBoundingBox.h
 * @brief Definition of the AxisAlignedBoundingBox class
 *
 * @date 10.01.2010
 * @author: Ole Rehmsen
 */

#ifndef AXISALIGNEDBOUNDINGBOX_H_
#define AXISALIGNEDBOUNDINGBOX_H_

#include "types.h"
#include <vector>

using namespace std;

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

	Point3 getCenter() const;
	Point3 getCorner(int i) const;
	const Point3& getMin() const;
	const Point3& getMax() const;

private:
	D3DXVECTOR3 min_;
	D3DXVECTOR3 max_;
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
