#include "Level.h"

Level::Level() {

}

bool Level::Init() {
	if (!LoadAssets())
		return false;

	srand((unsigned)time(NULL));

	// Hardcoded initialization
	std::list<Collectible> collectibles;
	std::list<Obstacle> obstacles;

	collectibles.push_back(Collectible(elements[0].list, 1, aiVector2D(2, 2), elements[0].size));
	collectibles.push_back(Collectible(elements[0].list, 1, aiVector2D(3, 2), elements[0].size));
	collectibles.push_back(Collectible(elements[0].list, 1, aiVector2D(4, 2), elements[0].size));
	collectibles.push_back(Collectible(elements[1].list, 2, aiVector2D(31, 2), elements[1].size));

	obstacles.push_back(Obstacle(elements[9].list, aiVector2D(10, 1), elements[9].size));
	obstacles.push_back(Obstacle(elements[10].list, aiVector2D(11, 6), elements[10].size));
	obstacles.push_back(Obstacle(elements[11].list, aiVector2D(24, 4), elements[11].size));

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
	if (!wall) {
		Utils::Log("Couldn't load asset file");
		return false;
	}
	wallList = Utils::GenerateList(wall);

	std::ifstream file("elements.txt", std::ios::in);

	if (!file) {
		Utils::Log("Couldn't load asset file");
		return false;
	}

	std::string filename;
	int xSize, ySize;
	while (file >> filename >> xSize >> ySize) {
		Element e;
		e.model = Utils::LoadAsset(filename.c_str());
		if (!e.model) {
			Utils::Log("Couldn't load asset");
			return false;
		}

		e.list = Utils::GenerateList(e.model);
		e.size.Set(xSize, ySize);
		elements.push_back(e);
	}

	file.close();
	return true;
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
}

int Level::CheckHit(aiVector2D bottomLeft, aiVector2D topRight) {

	for (std::list<Block>::iterator i = blockQueue.begin(); i != blockQueue.end(); ++i) {
		if (i->ObstacleHit(bottomLeft, topRight))
			return 3;
		int hit = i->CollectibleHit(bottomLeft, topRight);
		if (hit != 0)
			return hit;
	}
	return 0;
}

Level::~Level() {
	aiReleaseImport(wall);

	for (int i = 0; i < elements.size(); i++)
		aiReleaseImport(elements[i].model);
}
