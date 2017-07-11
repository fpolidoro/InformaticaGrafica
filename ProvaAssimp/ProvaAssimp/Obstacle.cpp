#include "Obstacle.h"

Obstacle::Obstacle(GLuint callList, aiVector2D bottomLeft, aiVector2D topRight) {
	this->callList = callList;
	this->bottomLeft = bottomLeft * 2;
	this->topRight = topRight * 2;
}

void Obstacle::Draw() {
	glPushMatrix();
	glTranslatef(bottomLeft.x, bottomLeft.y, 0.f);
	glCallList(callList);
	glPopMatrix();
}

void Obstacle::Move(float amount) {
	bottomLeft.x += amount;
	topRight.x += amount;
}

bool Obstacle::Hit(aiVector2D bottomLeft, aiVector2D topRight) {
	return !(bottomLeft.x > (this->bottomLeft).x
		|| topRight.x < (this->topRight).x
		|| bottomLeft.y >(this->bottomLeft).y
		|| topRight.y < (this->topRight).y);
}

Obstacle::~Obstacle() {
}
