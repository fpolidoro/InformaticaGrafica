#include "Block.h"

Block::Block(){}

Block::Block(GLuint callList) {
	this->callList = callList;
}

void Block::Init(float start) {
	this->position.Set(start, 0.f);
}

void Block::InitHardCoded(std::list<Obstacle> obstacles, std::list<Collectible> collectibles) {
	this->obstacles = obstacles;
	this->collectibles = collectibles;

	for (std::list<Obstacle>::iterator i = this->obstacles.begin(); i != this->obstacles.end(); ++i)
		i->Move(position.x);
	for (std::list<Collectible>::iterator i = this->collectibles.begin(); i != this->collectibles.end(); ++i)
		i->Move(position.x);
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

bool Block::ObstacleHit(aiVector2D lowLeft, aiVector2D upRight) {

	for each(Obstacle o in obstacles) 
		if (o.Hit(lowLeft, upRight))
			return true;

	return false;
}

int Block::CollectibleHit(aiVector2D lowLeft, aiVector2D upRight) {

	for each(Collectible c in collectibles)
		if (c.Hit(lowLeft, upRight))
			return c.type;

	return 0;
}

Block::~Block() {
}
