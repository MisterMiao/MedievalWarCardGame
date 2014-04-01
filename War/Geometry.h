#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <SDL.h>
#include <cmath>
#include "HelperFunctions.h"

struct Vector2;
struct Rectangle;

struct Vector2 {
	float X, Y; // The X and Y-coordinates of the vector

	// Vector2 objects for [0, 0] and [1, 1]
	static const Vector2 Zero;
	static const Vector2 One;

	// Operator overloading for 2D vector arithmetic
	bool operator==(const Vector2&) const; // Checks if a vector's coordinates are the same as another vector
	bool operator!=(const Vector2&) const;
	Vector2& operator+=(const Vector2&); // Adds another vector to this vector
	Vector2& operator-=(const Vector2&); // Subtracts another vector from this vector
	Vector2& operator*=(float); // Multiplies this vector by a scalar value
	const Vector2 operator+(const Vector2&) const;
	const Vector2 operator-(const Vector2&) const;
	const Vector2 operator*(float) const;

	// Vector2 functions
	float GetMagnitude();
	float GetAngleFromZero();
	float GetDotProduct(const Vector2&);
	float GetAngleBetweenVectors(Vector2&);
	void Rotate(float);
	void Normalize();
	void NegateX() { X *= -1; }
	void NegateY() { Y *= -1; }
	Vector2 GetBoundary(const Vector2&);
	Vector2 GetNormal();
	Vector2 Lerp(const Vector2&, float);

	Vector2(float x, float y) { X = x; Y = y; }
	Vector2(const SDL_Point& point) { X = (float)point.x; Y = (float)point.y; } // Copy constructor SDL_Point -> Vector2
	Vector2() { X = 0; Y = 0; }
};

struct Rectangle {
	float X, Y, Width, Height;

	bool Intersects(const Rectangle&) const; // Whether or not the rectangle is intersecting another rectangle
	bool Contains(float, float) const; // Whether or not the rectangle contains a point
	bool Contains(const Vector2&) const;

	Rectangle(float x, float y, float w, float h) { X = x; Y = y; Width = w; Height = h; }
	Rectangle(float w, float h) { X = 0; Y = 0; Width = w; Height = h; }
	Rectangle(const Vector2& pos, const Vector2& dim) { X = pos.X; Y = pos.Y; Width = dim.X; Height = dim.Y; }
	Rectangle(const Vector2& dim) { X = 0; Y = 0; Width = dim.X; Height = dim.Y; }
	Rectangle(const Rectangle& rect) { X = rect.X; Y = rect.Y; Width = rect.Width; Height = rect.Height; }
	Rectangle() { X = 0; Y = 0; Width = 0; Height = 0; }
};
#endif