#pragma once
#include "Block.h"

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <vector>

#define N_BLOCKS 2
#define START_POS -14.f
#define LENGTH 64.f
#define END_POS START_POS - LENGTH
#define N_ELEMENTS 12

class Level {
public:
	Level();

	bool Init();
	bool LoadAssets();
	void Move(float amount);
	void Draw();
	void NextBlock();
	int CheckHit(aiVector2D bottomLeft, aiVector2D topRight);

	~Level();

private:
	Block blockArray[N_BLOCKS];
	std::list<Block> blockQueue;

	struct Element {
		const aiScene* model;
		GLuint list;
		aiVector2D size;
	};
	std::vector<Element> elements;

	const aiScene* wall;
	GLuint wallList;
};

