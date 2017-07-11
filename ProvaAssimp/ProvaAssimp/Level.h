#pragma once
#include "Block.h"

#include <stdlib.h>
#include <ctime>

#define N_BLOCKS 2
#define START_POS -14.f
#define LENGTH 64.f
#define END_POS START_POS - LENGTH

class Level {
public:
	Level();

	bool Init();
	bool LoadAssets();
	void Move(float amount);
	void Draw();
	void NextBlock();

	~Level();

private:
	Block blockArray[N_BLOCKS];
	std::list<Block> blockQueue;

	const aiScene* wall;
	const aiScene* coin;
	const aiScene* life;
	const aiScene* platform1;
	const aiScene* platform2;
};

