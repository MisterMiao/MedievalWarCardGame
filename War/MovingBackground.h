#ifndef MOVINGBACKGROUND_H
#define MOVINGBACKGROUND_H
#include <vector>
#include "Engine.h"

// Lynda provided a class similar to this in our Monogame course. I ported it to C++ with only the features I needed.
class MovingBackground {
private:
	struct BackgroundLayer {
		Texture2D image;
		Vector2 position;
		Vector2 imageSize;
		float moveRate;
		float rotationRate;
		float rotation;

		BackgroundLayer();
	};

	std::vector<BackgroundLayer*> layerList;
	bool moving;
	bool moveHorizontally;
	Vector2 windowSize;
public:
	// Add a layer to the background, and the rate it should move at
	void AddLayer(Texture2D, float = 0.0F, float = 0.0F);

	// Set it to move horizontally or vertically
	void SetMovement(bool moveHorizontally) { this->moveHorizontally = moveHorizontally; }
	// Start the background moving
	void Start() { moving = true; }
	// Stop the background moving
	void Stop() { moving = false; }

	void Update(const float);
	void Draw(ContentManager*);

	MovingBackground(const Vector2&);
	~MovingBackground();
};
#endif