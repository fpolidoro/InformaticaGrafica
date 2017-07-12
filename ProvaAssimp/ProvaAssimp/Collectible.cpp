#include "Collectible.h"

Collectible::Collectible(GLuint callList, int type, aiVector2D bottomLeft, aiVector2D size) {
	this->callList = callList;
	this->type = type;
	this->bottomLeft = bottomLeft * 2.f;
	this->topRight = (size + bottomLeft) * 2.f;
}

void Collectible::Draw() {
	glPushMatrix();
	glTranslatef(bottomLeft.x, bottomLeft.y, 0.f);
	glutSolidCube(0.5f);
	glCallList(callList);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(topRight.x, topRight.y, 0.f);
	glutSolidCube(0.5f);
	glPopMatrix();
}

void Collectible::Move(float amount) {
	bottomLeft.x += amount;
	topRight.x += amount;
}

bool Collectible::Hit(aiVector2D bottomLeft, aiVector2D topRight) {
	return !(bottomLeft.x >(this->topRight).x
		|| topRight.x < (this->bottomLeft).x
		|| bottomLeft.y >(this->topRight).y
		|| topRight.y < (this->bottomLeft).y);
}

Collectible::~Collectible() {
}
