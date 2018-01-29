#pragma once
#include "ofMain.h"
#include "FractalNoise.h"

class VisCreator
{
public:
	VisCreator();
	~VisCreator();

	ofTexture* getTexture();

private:
	ofFbo renderFbo;
	ofTexture tex;

	FractalNoise noise;
};

