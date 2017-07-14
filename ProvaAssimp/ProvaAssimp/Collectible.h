#pragma once
#include "Utils.h"
#include <GL\glut.h>

class Collectible {
public:
	Collectible(GLuint callList, int type, aiVector2D bottomLeft, aiVector2D size);

	void Draw();
	void Move(float amount);
	bool Hit(aiVector2D bottomLeft, aiVector2D topRight);

	int type;

	~Collectible();

private:
	GLuint callList;

	aiVector2D bottomLeft;
	aiVector2D topRight;
	float rotation = 0.f;
};

