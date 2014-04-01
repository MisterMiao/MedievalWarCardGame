#include "Geometry.h"

const Vector2 Vector2::Zero = Vector2();
const Vector2 Vector2::One = Vector2(1, 1);

bool Vector2::operator==(const Vector2& vec) const {
	return (X == vec.X) && (Y == vec.Y);
}

bool Vector2::operator!=(const Vector2& vec) const {
	return !(*this == vec);
}

Vector2& Vector2::operator+=(const Vector2& vec) {
	X += vec.X;
	Y += vec.Y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& vec) {
	X -= vec.X;
	Y -= vec.Y;
	return *this;
}

Vector2& Vector2::operator*=(float scalar) {
	X *= scalar;
	Y *= scalar;
	return *this;
}

const Vector2 Vector2::operator+(const Vector2& vec) const {
	return Vector2(*this) += vec;
}

const Vector2 Vector2::operator-(const Vector2& vec) const {
	return Vector2(*this) -= vec;
}

const Vector2 Vector2::operator*(float scalar) const {
	return Vector2(*this) *= scalar;
}

float Vector2::GetMagnitude() {
	return sqrt((X * X) + (Y * Y));
}

float Vector2::GetAngleFromZero() {
	float angle = (float)(atan2(Y, X) * 180 / M_PI);
	angle += angle < 0 ? 360 : 0;
	return angle;
}

float Vector2::GetDotProduct(const Vector2& vec) {
	return (X * vec.X) + (Y * vec.Y);
}

float Vector2::GetAngleBetweenVectors(Vector2& vec) {
	return acos(GetDotProduct(vec) / (GetMagnitude() * vec.GetMagnitude())) * (float)(180 / M_PI);
}

void Vector2::Rotate(float angle) {
	float rads = angle * (float)(M_PI / 180);
	float x = X, y = Y;
	X = (x * cos(rads)) - (y * sin(rads));
	Y = (x * sin(rads)) + (y * cos(rads));
}

void Vector2::Normalize() {
	float magnitude = GetMagnitude();
	X /= magnitude;
	Y /= magnitude;
}

Vector2 Vector2::GetBoundary(const Vector2& vec) {
	return vec - *this;
}

Vector2 Vector2::GetNormal() {
	return Vector2(Y, -X);
}

Vector2 Vector2::Lerp(const Vector2& vec, float t) {
	Vector2 boundary = GetBoundary(vec);
	boundary *= t;
	return *this + boundary;
}