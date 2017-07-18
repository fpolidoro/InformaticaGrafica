#include "Obstacle.h"

Obstacle::Obstacle(GLuint callList, aiVector2D bottomLeft, aiVector2D size) {
	this->callList = callList;
	this->bottomLeft = bottomLeft * 2.f;
	this->topRight = (size + bottomLeft) * 2.f;
}

void Obstacle::Draw() {
	glPushMatrix();
	glTranslatef(bottomLeft.x, bottomLeft.y, 0.f);
	glCallList(callList);
	glPopMatrix();

	//glPushMatrix();
	//glTranslatef(bottomLeft.x, bottomLeft.y, 0.f);
	//glutSolidCube(0.5f);
	//glPopMatrix();

	//glPushMatrix();
	//glTranslatef(topRight.x, topRight.y, 0.f);
	//glutSolidCube(0.5f);
	//glPopMatrix();
}

void Obstacle::Move(float amount) {
	bottomLeft.x += amount;
	topRight.x += amount;
}

bool Obstacle::Hit(aiVector2D bottomLeft, aiVector2D topRight) {
	return !(bottomLeft.x > (this->topRight).x
		|| topRight.x < (this->bottomLeft).x
		|| bottomLeft.y > (this->topRight).y
		|| topRight.y < (this->bottomLeft).y);
}

Obstacle::~Obstacle() {
}
