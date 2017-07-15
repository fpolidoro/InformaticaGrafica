#pragma once
#include "Block.h"

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>

#define N_BLOCKS 15
#define START_POS -14.f
#define LENGTH 64.f
#define END_POS START_POS - LENGTH
#define N_ELEMENTS 12

class Level {
public:
	Level();

	bool Init();
	bool LoadAssets();
	bool GenerateBlocks();
	void Start();
	void Move(float amount);
	void Draw();
	void NextBlock();
	int CheckHit(aiVector2D bottomLeft, aiVector2D topRight);

	~Level();

private:
	std::vector<Block> blockArray;
	std::list<Block> blockQueue;

	struct Element {
		const aiScene* model;
		GLuint list;
		aiVector2D size;
	};
	std::vector<Element> elements;

	const aiScene* wall;
	GLuint wallList;
	const aiScene* background;
	GLuint backgroundList;

	int test = 0;
};

