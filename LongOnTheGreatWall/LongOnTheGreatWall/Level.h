#pragma once
#include "Block.h"

#include <stdlib.h>
#include <ctime>
#include <fstream>
#include <vector>
#include <sstream>

#define N_BLOCKS 10
#define START_POS -14.f
#define LENGTH 64.f
#define END_POS START_POS - LENGTH
#define N_ELEMENTS 12
#define VELOCITY 12.f
#define VELOCITY_INCREASE 2.f

#define BG_VELOCITY_FACTOR 4.f
#define N_BG 4
#define BG_Z_POS -7.f
#define BG_X_POS -8.f
#define BG_Y_POS -4.8f
#define BG_SCALE 1.5f
#define BG_LENGTH (LENGTH / 2.f * BG_SCALE)

#define N_BLOCKS_TO_FASTER 5

class Level {
public:
	bool Init();
	bool LoadAssets();
	bool GenerateBlocks();
	void Start();

	void Move(float deltaTime);
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
	GLuint skyList;
	GLuint bgList[4];

	float bgArray[4];
	
	int blockCount;
	float velocity;

	//int test = 0;
};

