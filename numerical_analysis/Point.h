#pragma once

template<typename T>
struct Point2D {
	T x, y;

	explicit Point2D(const T& _x, const T& _y)
		: x(_x), y(_y) {}
};

template<typename T>
struct Point3D {
	T x, y, z;

	explicit Point3D(const T& _x, const T& _y, const T& _z)
		: x(_x), y(_y), z(_z) {}
};