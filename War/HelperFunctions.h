#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <SDL.h>

inline SDL_Point CreatePoint(int, int);
inline SDL_Color CreateColor(int, int, int, int = 255);
inline SDL_Rect CreateRect(int, int, Uint32, Uint32);

SDL_Point CreatePoint(int x, int y) {
	SDL_Point point = SDL_Point();
	point.x = x;
	point.y = y;

	return point;
}

SDL_Color CreateColor(int r, int g, int b, int a) {
	SDL_Color color = SDL_Color();
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;

	return color;
}

SDL_Rect CreateRect(int x, int y, Uint32 w, Uint32 h) {
	SDL_Rect rect = SDL_Rect();
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	return rect;
}
#endif