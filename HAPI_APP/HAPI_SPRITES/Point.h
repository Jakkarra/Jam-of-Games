#pragma once
/*
	Point type
	A basic integer point class
	Performance wise is best by value not const reference
*/

namespace HAPISPACE {

	class Point final
	{
	public:
		int x;
		int y;

		// Construct from optional x and y values
		Point(int xin = 0, int yin = 0) : x(xin), y(yin) {}

		// Add a point to this
		Point& operator+=(Point rhs)
		{
			x += rhs.x;
			y += rhs.y;
			return *this;
		}

		// Subtract a point from this
		Point& operator-=(Point rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			return *this;
		}

		// integer scale this point
		Point& operator*=(int mult)
		{
			x *= mult;
			y *= mult;
			return *this;
		}

		// Negate
		Point operator-() const
		{
			return Point{ -x,-y };
		}

		// Distance to other point from this point squared
		int DistanceBetweenSquared(const Point &other) const
		{
			return (other.x - x)*(other.x - x) + (other.y - y)*(other.y - y);
		}

		// Distance to other point from this point
		float DistanceBetween(const Point &other) const
		{
			return sqrt((float)DistanceBetweenSquared(other));
		}

		// Manhattan distance to other point from this point
		int ManhattanDistanceBetween(const Point &p2) const
		{
			return abs(x - p2.x) + abs(y - p2.y);
		}

		// Creates a random point between min and max
		static Point Random(int minX,int maxX, int minY, int maxY)
		{
			return Point(minX + rand() % (maxX - minX), minY + rand() % (maxY - minY));
		};

		// Creates a random point from 0 to max
		static Point Random(int maxX, int maxY)
		{
			return Random(0,maxX,0,maxY);
		};

		friend std::ostream& operator<<(std::ostream& os, const Point& p);
		// No need for assignment operator or copy or move since default bitwise ones work fine (its just 8 bytes)
	};

	// Can be used with cout to output values to output pane and log
	inline std::ostream& operator<<(std::ostream& os, const Point& p)
	{
		os << "X: " << std::to_string(p.x) << " Y: " << std::to_string(p.y);
		return os;
	}

	// Equals
	inline bool operator==(Point lhs, Point rhs)
	{ 
		return (lhs.x == rhs.x && lhs.y == rhs.y); 
	}
	
	// Does not equal
	inline bool operator!=(Point lhs, Point rhs)
	{
		return !operator==(lhs, rhs); 
	}

	// Other comparison operators like <,>,<=,>= are meaningless when dealing with points
	// Unary operators are also meaningless

	// Add two points
	inline Point operator+(Point lhs, const Point &rhs)
	{
		lhs += rhs;
		return lhs;
	}	

	// Subtract two points
	inline Point operator-(Point lhs, const Point &rhs)
	{
		lhs -= rhs;
		return lhs;
	}
}