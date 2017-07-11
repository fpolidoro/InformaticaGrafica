#include "Collectible.h"

Collectible::Collectible(GLuint callList, int type, aiVector2D bottomLeft, aiVector2D topRight) {
	this->callList = callList;
	this->type = type;
	this->bottomLeft = bottomLeft * 2;
	this->topRight = topRight * 2;
}

void Collectible::Draw() {
	glPushMatrix();
	glTranslatef(bottomLeft.x, bottomLeft.y, 0.f);
	glCallList(callList);
	glPopMatrix();
}

void Collectible::Move(float amount) {
	bottomLeft.x += amount;
	topRight.x += amount;
}

bool Collectible::Hit(aiVector2D bottomLeft, aiVector2D topRight) {

	return !(bottomLeft.x > (this->bottomLeft).x
		|| topRight.x < (this->topRight).x
		|| bottomLeft.y >(this->bottomLeft).y
		|| topRight.y < (this->topRight).y);
}

Collectible::~Collectible() {
}
