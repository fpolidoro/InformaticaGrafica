#pragma once
#include "Obstacle.h"
#include "Collectible.h"
#include <list>

class Block {
public:
	Block();
	Block(GLuint callList);

	void Init(float x);
	void InitHardCoded(std::list<Obstacle> obstacles, std::list<Collectible> collectibles);
	void Move(float amount);
	void Draw();

	bool ObstacleHit(aiVector2D lowLeft, aiVector2D upRight);
	int CollectibleHit(aiVector2D lowLeft, aiVector2D upRight);

	aiVector2D position;

	~Block();

private:
	std::list<Obstacle> obstacles;
	std::list<Collectible> collectibles;

	GLuint callList;	
};
