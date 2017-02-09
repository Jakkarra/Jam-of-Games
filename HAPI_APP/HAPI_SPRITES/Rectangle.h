#pragma once
/*
	Rectangle type
	A simple rectangle class with left, right, top and bottom members. 
	Bottom is > top (y down the screen).
	Performance wise is best by value not const reference
*/

#include <array>
#include "Point.h"
#include "Line.h"

namespace HAPISPACE {

	class Rectangle final
	{
	public:
		// Default values are for an invalid rectangle
		int left{ 0 }, right{ 0 }, top{ 0 }, bottom{ 0 };

		// Constructors
		Rectangle() = default;

		// Construct from values for left, right, top and bottom
		Rectangle(int leftIn, int rightIn, int topIn, int bottomIn) : left(leftIn), right(rightIn), top(topIn), bottom(bottomIn) {}

		// Construct from a top left point and bottom right point
		Rectangle(Point topLeft, Point bottomRight) : left(topLeft.x), right(bottomRight.x), top(topLeft.y), bottom(bottomRight.y) {}

		// Construct from top left and width and height
		Rectangle(Point topLeft, int width, int height) : left(topLeft.x), right(topLeft.x + width), top(topLeft.y), bottom(topLeft.y + height) {}

		// Construct from width and height
		Rectangle(int width, int height) : left(0), right(width), top(0), bottom(height) {}

		// Returns the width of the rectagle
		int Width() const { return right - left; }

		// Return the height of the rectange
		int Height() const { return bottom - top; }

		// A valid rectangle has right>left and bottom>top (assuming +y is down) 
		bool IsValid() const { if (left >= right || top >= bottom) return false; return true; }

		// Does this rectangle completely contain the other?		
		bool Contains(Rectangle other) const {return (other.left >= left && other.right < right && other.top >= top && other.bottom < bottom);}
		
		// Does this rectangle completely contain the point
		bool Contains(Point other) const {return !(other.x < left || other.x >= right || other.y < top || other.y >= bottom);}

		// Is this rectangle completely outside of the other?
		bool OutsideOf(Rectangle other) const {return (right < other.left || bottom < other.top || left >= other.right || top >= other.bottom);}

		// Do the rectangles intersect?
		bool Intersects(Rectangle other) const {return !(other.left > right || other.right < left|| other.top > bottom|| other.bottom < top);}

		// Translate rectangle
		void Translate(int dx, int dy) { left += dx; right += dx; top += dy; bottom += dy; }

		// Translate rectangle using a point
		void Translate(Point d) { left += d.x; right += d.x; top += d.y; bottom += d.y; }

		// Translate to origin
		void TranslateToOrigin() { right -= left; left = 0; bottom -= top; top = 0; }

		// Expand leaves left, top the same
		void Expand(Point d) { right += d.x; bottom += d.y; }

		// Clip this rectangle against the other
		void ClipTo(Rectangle other);

		// Point class getters for individual access to the 4 corners of the rectangle
		Point TopLeft() const { return Point(left, top); }
		Point TopRight() const { return Point(right, top); }
		Point BottomLeft() const { return Point(left, bottom); }
		Point BottomRight() const { return Point(right, bottom); }
		Point Dimensions() const { return Point(Width(), Height()); }
		
		// Returns all 4 corners of the rectangle in clockwise order
		// Note: once Vis fully supports c++14 can remove extra bracket
		std::vector<Point> GetCorners() const {	return{ { {TopLeft()},{TopRight()},{BottomRight()},{BottomLeft()} } };}

		// Returns 4 lines representing the outline of this rectangle
		std::vector<Line> GetOutline() const {return{ { {TopLeft(),TopRight()},{TopRight(),BottomRight()},{ BottomRight(),BottomLeft() },{ BottomLeft(),TopLeft() } } };}

		int DiagonalLengthSquared() const {return (right - left)*(right - left) + (bottom - top)*(bottom - top);}

		// Creates a random rectangle between min and max points
		// Note max must be > min by at least 2
		static Rectangle Random(Point min, Point max)
		{
			Point topLeft(Point::Random(min.x, max.x - 2, min.y, max.y - 2));
			Point bottomRight(Point::Random(topLeft.x + 1, max.x, topLeft.y + 1, max.y));
			return Rectangle(topLeft, bottomRight);
		}

		friend std::ostream& operator<<(std::ostream& os, const Rectangle& r);
	};

	// Can be used with cout to output values to output pane and log
	inline std::ostream& operator<<(std::ostream& os, const Rectangle& r)
	{
		os << "Left:" + std::to_string(r.left) + " Right:" + std::to_string(r.right) +
			" Top:" + std::to_string(r.top) + " Bottom:" + std::to_string(r.bottom) +
			" (Width:" + std::to_string(r.Width()) + " Height:" + std::to_string(r.Height())+")";
		return os;
	}
	
}