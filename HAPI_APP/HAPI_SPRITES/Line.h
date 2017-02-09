#pragma once
/*
	Line type
*/

#include "Point.h"

namespace HAPISPACE {

	// A basic integer line class
	// Performance wise it is best to pass by value not const reference
	class Line final
	{
	public:
		Point p1, p2;

		// Construct from two points
		Line(Point p1in, Point p2in) : p1(std::move(p1in)), p2(std::move(p2in)) {}
		
		// Construct from x,y values for each point
		Line(int x1, int y1, int x2, int y2) : p1(x1,y1), p2(x2,y2) {}

		// Translates this line by d
		void Translate(const Point &d) {p1 += d;	p2 += d;}

		// Returns the manhattan length of the line
		int ManhattanLength() const {return p1.ManhattanDistanceBetween(p2);}

		// Returns the length squared (useful for optimisation)
		float LengthSquared() const { return p1.DistanceBetween(p2); }

		// Returns the actual length of the line
		float Length() const { return sqrt(LengthSquared()); }

		friend std::ostream& operator<<(std::ostream& os, const Line& l);
	};

	// Can be used with cout to output values to output pane and log
	inline std::ostream& operator<<(std::ostream& os, const Line& l)
	{
		os << "P1: " << l.p1 << " P2: " << l.p2;
		return os;
	}

	// Equivelance operator
	inline bool operator==(const Line& lhs, const Line& rhs)
	{
		return (lhs.p1 == rhs.p1 && lhs.p2 == rhs.p2);
	}

	// Does not equal operator
	inline bool operator!=(const Line& lhs, const Line& rhs)
	{
		return !operator==(lhs, rhs);
	}
}