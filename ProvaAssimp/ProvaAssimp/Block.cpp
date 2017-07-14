#include "Block.h"

Block::Block(GLuint callList) {
	this->callList = callList;
	position.Set(0.f, 0.f);
}

Block::Block(GLuint callList, std::list<Obstacle> obstacles, std::list<Collectible> collectibles) {
	this->callList = callList;
	this->obstacles = obstacles;
	this->collectibles = collectibles;
	position.Set(0.f, 0.f);
}

void Block::Move(float amount) {
	position.x += amount;

	for (std::list<Obstacle>::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
		i->Move(amount);
	for (std::list<Collectible>::iterator i = collectibles.begin(); i != collectibles.end(); ++i)
		i->Move(amount);
}

void Block::Draw() {

	glPushMatrix();
	glTranslatef(position.x, position.y, 0.f);
	glCallList(callList);
	glPopMatrix();

	for (std::list<Obstacle>::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
		i->Draw();
	for (std::list<Collectible>::iterator i = collectibles.begin(); i != collectibles.end(); ++i)
		i->Draw();
}

bool Block::ObstacleHit(aiVector2D bottomLeft, aiVector2D topRight) {

	for (std::list<Obstacle>::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
		if (i->Hit(bottomLeft, topRight)) {
			return true;
		}

	return false;
}

int Block::CollectibleHit(aiVector2D bottomLeft, aiVector2D topRight) {

	for (std::list<Collectible>::iterator i = collectibles.begin(); i != collectibles.end(); ++i)
		if (i->Hit(bottomLeft, topRight)) {
			int type = i->type;
			collectibles.erase(i);
			return type;
		}

	return 0;
}

Block::~Block() {
}
