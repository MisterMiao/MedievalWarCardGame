#include "Engine.h"

static ContentManager* instance;

ContentManager* ContentManager::GetInstance() {
	if (instance == nullptr)
		instance = new ContentManager();

	return instance;
}

void ContentManager::DestroyContentManager(ContentManager*& contentManagerPointer) {
	if (instance) {
		delete instance;
		instance = nullptr;
		contentManagerPointer = nullptr;
	}
}

ContentManager::ContentManager() {
	rootDirectory = "Content";
}

ContentManager::~ContentManager() {
	Unload();
}

Texture2D ContentManager::LoadTexture(string _path) {
	string path = rootDirectory + "/Images/" + _path; // Append the root directory to the path

	// Check if the texture already exists in the map, if so, return its iterator
	if (textures.find(_path) != textures.end())
		return textures.find(_path);

	SDL_Texture* tex = nullptr;
	tex = IMG_LoadTexture(renderer, path.c_str()); // Load the texture
	if (tex == nullptr) // If the texture failed to load, throw an error
		throw runtime_error("Failed to load image: " + path + IMG_GetError());

	textures.insert(make_pair(_path, tex)); // Add the texture to the ContentManager's map

	return textures.find(_path);
}

Font ContentManager::LoadFont(string _path, int fontSize) {
	string path = rootDirectory + "/Fonts/" + _path;
	string key = _path + ":" + to_string(fontSize); // Append fontsize to path to account for duplicate typefaces

	if (fonts.find(key) != fonts.end())
		return fonts.find(key);

	TTF_Font* font = nullptr;
	font = TTF_OpenFont(path.c_str(), fontSize);
	if (font == nullptr)
		throw runtime_error("Failed to load font: " + path + TTF_GetError());

	fonts.insert(make_pair(key, font));

	return fonts.find(key);
}

Music ContentManager::LoadMusic(string _path) {
	string path = rootDirectory + "/Sounds/" + _path;

	if (music.find(_path) != music.end())
		return music.find(_path);

	Mix_Music* mus = nullptr;
	mus = Mix_LoadMUS(path.c_str());
	if (mus == nullptr)
		throw runtime_error("Failed to load music: " + path + Mix_GetError());

	music.insert(make_pair(_path, mus));

	return music.find(_path);
}

Sound ContentManager::LoadSound(string _path) {
	string path = rootDirectory + "/Sounds/" + _path;

	if (strcmp(_path.c_str(), "") == 0) // If a file is not specified return NULL_SOUND aka sounds.end
		return NULL_SOUND;

	if (sounds.find(_path) != sounds.end())
		return sounds.find(_path);

	Mix_Chunk* sound = nullptr;
	sound = Mix_LoadWAV(path.c_str());
	if (sound == nullptr)
		throw runtime_error("Failed to load sound: " + path + Mix_GetError());

	sounds.insert(make_pair(_path, sound));

	return sounds.find(_path);
}

void ContentManager::DrawTexture(Texture2D texture, const Vector2* _position, const Rectangle* _srcRect, float angle, float scale, const Vector2* _origin, SDL_RendererFlip flip) {
	SDL_Point* position = _position ? &CreatePoint((int)_position->X, (int)_position->Y) : &CreatePoint(0, 0); // Create a SDL_Point from a Vector2
	SDL_Point* origin = _origin ? &CreatePoint((int)_origin->X, (int)_origin->Y) : nullptr;
	SDL_Rect* srcRect = _srcRect ? &CreateRect((int)_srcRect->X, (int)_srcRect->Y, (Uint32)_srcRect->Width, (Uint32)_srcRect->Height) : nullptr; // Create a SDL_Rectangle from a Rectangle
	SDL_Rect* destRect = nullptr;

	if (srcRect) // If the source rectangle is not null, set the destination rectangle based on it
		destRect = &CreateRect(position->x, position->y, (int)(srcRect->w * scale), (int)(srcRect->h * scale));
	else { // Otherwise use the entire image
		int w = 0, h = 0;
		SDL_QueryTexture(texture->second, NULL, NULL, &w, &h); // Get the width and height of the texture
		destRect = &CreateRect(position->x, position->y, (int)(w * scale), (int)(h * scale));
	}

	if (angle == 0 && flip == 0)
		SDL_RenderCopy(renderer, texture->second, srcRect, destRect);
	else
		SDL_RenderCopyEx(renderer, texture->second, srcRect, destRect, angle, origin, flip);
}

void ContentManager::DrawString(string text, const Vector2* _position, Font _font, SDL_Color color, bool centerOnPosition) {
	TTF_Font* font = _font->second; // Get the actual font
	SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), color); // Create a surface from the font
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf); // Create a texture
	SDL_FreeSurface(surf); // Deallocate the surface

	int w = 0, h = 0;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h); // Get the width and height of the texture
	SDL_Point* position = _position ? &CreatePoint((int)_position->X, (int)_position->Y) : &CreatePoint(0, 0);
	SDL_Rect* srcRect = &CreateRect(0, 0, w, h);
	SDL_Rect* destRect = &CreateRect(position->x - (centerOnPosition ? w / 2 : 0), position->y - (centerOnPosition ? h / 2 : 0), w, h);

	SDL_RenderCopy(renderer, tex, srcRect, destRect);
	SDL_DestroyTexture(tex);
}

Vector2 ContentManager::MeasureString(string text, Font _font) {
	int w = 0, h = 0;
	TTF_SizeText(_font->second, text.c_str(), &w, &h);

	return Vector2((float)w, (float)h);
}

void ContentManager::Unload() {
	for (Texture2D i = textures.begin(); i != textures.end(); i++) // Loop through and free all textures
		SDL_DestroyTexture(i->second);
	for (Font i = fonts.begin(); i != fonts.end(); i++) // Loop through and closes all fonts
		TTF_CloseFont(i->second);
	for (Music i = music.begin(); i != music.end(); i++) // Loop through and free all music
		Mix_FreeMusic(i->second);
	for (Sound i = sounds.begin(); i != sounds.end(); i++) // Loop through and free all sound effects
		Mix_FreeChunk(i->second);
}