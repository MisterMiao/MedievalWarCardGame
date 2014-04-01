#include "Geometry.h"

bool Rectangle::Intersects(const Rectangle& rect) const {
	return !(X <= rect.X + rect.Width) && !(X + Width >= rect.X) && !(Y <= rect.Y + rect.Height) && !(Y + Height >= rect.Y);
}

bool Rectangle::Contains(float x, float y) const {
	return x <= X + Width && x >= X && y <= Y + Height && y >= Y;
}

bool Rectangle::Contains(const Vector2& vec) const {
	return Contains(vec.X, vec.Y);
}