#pragma once
#include "Utils.h"
#include <GL\glut.h>

class Obstacle {
public:
	Obstacle(GLuint callList, aiVector2D bottomLeft, aiVector2D size);

	void Draw();
	void Move(float amount);
	bool Hit(aiVector2D bottomLeft, aiVector2D topRight);

	~Obstacle();

private:
	GLuint callList;

	aiVector2D bottomLeft;
	aiVector2D topRight;
};

