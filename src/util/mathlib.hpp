
/*
 *
 *	An area to put all that good delicious math that can work crossgame, or in some of the framework!
 *		Dont put anything game specific here please!
 *			-Onee
 *
 */

#pragma once

#include <math.h> // This is a mathlib, did you think we didnt need math?

// Macro for getting the size of an array, Uses the size of first element then compares it to the size of the entire array. This is optimised out.
#define GET_ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))

// Pi stuff
#define PI 3.14159265
#define RADPI 57.295779513082f

// Struct for point in 3d space
class CatVector {
public:
	CatVector(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}
	float x, y, z;
	// So we can do math with our catvectors easily
	inline CatVector operator+(CatVector value) const { return CatVector(x + value.x, y + value.y, z + value.z); }
	inline CatVector operator-(CatVector value) const { return CatVector(x - value.x, y - value.y, z - value.z); }
	inline CatVector operator*(CatVector value) const { return CatVector(x * value.x, y * value.y, z * value.z); }
	inline CatVector operator/(CatVector value) const { return CatVector(x / value.x, y / value.y, z / value.z); }
	inline CatVector operator*(float value) 	   const { return CatVector(x * value, y * value, z * value); }
	inline CatVector operator/(float value)	   const { return CatVector(x / value, y / value, z / value); }
	inline bool 	operator==(CatVector value) const { return value.x == x && value.y == y && value.z == z; }
	inline bool 	operator!=(CatVector value) const { return value.x != x || value.y != y || value.z != z; }
	// Used to get the distance between 2 vectors
	inline float DistTo(CatVector end) const { return sqrt((pow(x - end.x, 2) + pow(y - end.y, 2) + pow(z - end.z, 2))); }
};

// Used to store points in a box
class CatBox {
public:
	CatBox(CatVector min = CatVector(), CatVector max = CatVector()) : min(min), max(max) {};
	CatVector min, max;

	inline void GetPoints(CatVector(&points)[8]) const { // Used to get out all 8 points from our box, Be sure to pass an array with at least 8 values
		float x, y, z;
		x = max.x - min.x;
		y = max.y - min.y;
		z = max.z - min.z;
		points[0] = min;
		points[1] = min + CatVector(x, 0, 0);
		points[2] = min + CatVector(x, y, 0);
		points[3] = min + CatVector(0, y, 0);
		points[4] = min + CatVector(0, 0, z);
		points[5] = min + CatVector(x, 0, z);
		points[6] = min + CatVector(0, y, z);
		points[7] = min + CatVector(x, y, z);
	}
	inline CatVector GetCenter() const { return (min + max) * 0.5; }
	inline bool operator==(CatBox value) const { return value.min == min && value.max == max; }
	inline bool operator!=(CatBox value) const { return value.min != min || value.max != max; }
};

// Struct for point in 4d space
class CatVector4 {
public:
	CatVector4(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f, float _a = 0.0f) : x(_x), y(_y), z(_z), a(_a) {}
	float x, y, z, a;

	// So we can do math with our catvectors easily
  inline CatVector4 operator*(float value)	const { return CatVector4(x * value, y * value, z * value, a * value); }
  inline CatVector4 operator/(float value)	const { return CatVector4(x / value, y / value, z / value, a / value); }
	inline bool operator==(CatVector4 value) const { return value.x == x && value.y == y && value.z == z && value.a == a; }
	inline bool operator!=(CatVector4 value) const { return value.x != x || value.y != y || value.z != z || value.a != a; }
};

namespace util {

// Input the angles of your player, the vector position of your camera, and the destination point and it returns fov value
float GetFov(CatVector orig_angle, CatVector eye_position, CatVector dest_point);
// Returns angles used to aim at an object from a point, to another
CatVector VectorAngles(CatVector src_point, CatVector dest_point);
// Give it a position, angles to go in, and how far to extend, then it returns a vector in that direction
CatVector ExtendLine(CatVector src_pos, CatVector src_angles, float extend_amt);

}
