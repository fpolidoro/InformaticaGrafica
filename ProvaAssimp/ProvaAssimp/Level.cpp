#include "Level.h"

Level::Level() {
	srand((unsigned)time(NULL));
}

bool Level::Init() {
	if (!LoadAssets())
		return false;

	GLuint wallList = Utils::GenerateList(wall);
	GLuint coinList = Utils::GenerateList(coin);
	GLuint lifeList = Utils::GenerateList(life);
	GLuint platform1List = Utils::GenerateList(platform1);
	GLuint platform2List = Utils::GenerateList(platform2);

	// Hardcoded initialization

	std::list<Collectible> collectibles;
	std::list<Obstacle> obstacles;

	collectibles.push_back(Collectible(coinList, 1, aiVector2D(2, 2), aiVector2D(3, 3)));
	collectibles.push_back(Collectible(coinList, 1, aiVector2D(3, 2), aiVector2D(4, 3)));
	collectibles.push_back(Collectible(coinList, 1, aiVector2D(4, 2), aiVector2D(5, 3)));
	collectibles.push_back(Collectible(lifeList, 2, aiVector2D(31, 2), aiVector2D(32, 3)));

	obstacles.push_back(Obstacle(platform1List, aiVector2D(10, 3), aiVector2D(12, 4)));
	obstacles.push_back(Obstacle(platform2List, aiVector2D(24, 2), aiVector2D(26, 4)));


	Block b1(wallList), b2(wallList);
	b1.Init(START_POS);
	b2.Init(START_POS + LENGTH);
	b2.InitHardCoded(obstacles, collectibles);
	blockArray[0] = b1;
	blockArray[1] = b2;
	blockQueue.push_back(b1);
	blockQueue.push_back(b2);

	return true;
}

bool Level::LoadAssets() {
	wall = Utils::LoadAsset("Models\\muro.obj");
	coin = Utils::LoadAsset("Models\\moneta.obj");
	life = Utils::LoadAsset("Models\\yinyang.obj");
	platform1 = Utils::LoadAsset("Models\\impalcatura1.obj");
	platform2 = Utils::LoadAsset("Models\\impalcatura2.obj");

	if (!wall || !coin || !life || !platform1 || !platform2) {
		fprintf(stderr, "Couldn't load assets");
		return false;
	};
}

void Level::Move(float amount) {

	for (std::list<Block>::iterator i = blockQueue.begin(); i != blockQueue.end(); ++i)
		i->Move(amount);

	if (blockQueue.front().position.x < END_POS)
		NextBlock();
}

void Level::Draw() {
	for (std::list<Block>::iterator i = blockQueue.begin(); i != blockQueue.end(); ++i)
		i->Draw();
}

void Level::NextBlock() {
	int n = rand() % N_BLOCKS;

	Block b = blockArray[n];
	b.Init(blockQueue.back().position.x + LENGTH);
	blockQueue.push_back(b);
	blockQueue.pop_front();
	std::cout << n << std::endl;
}

Level::~Level() {
	aiReleaseImport(wall);
	aiReleaseImport(coin);
	aiReleaseImport(life);
	aiReleaseImport(platform1);
	aiReleaseImport(platform2);
}
