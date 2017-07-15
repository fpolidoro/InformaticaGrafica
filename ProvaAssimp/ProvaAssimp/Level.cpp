#include "Level.h"

Level::Level() {

}

bool Level::Init() {
	if (!LoadAssets())
		return false;

	if (!GenerateBlocks())
		return false;

	return true;
}

bool Level::LoadAssets() {
	;
	if (!(background = Utils::LoadAsset("Models\\sfondo.obj")))
		return false;
	backgroundList = Utils::GenerateList(background);

	if (!(wall = Utils::LoadAsset("Models\\muro.obj")))
		return false;
	wallList = Utils::GenerateList(wall);

	std::ifstream file("Levels\\elements.txt", std::ios::in);

	if (!file) {
		Utils::Log("Couldn't load asset file");
		return false;
	}

	std::string filename;
	int xSize, ySize;
	while (file >> filename >> xSize >> ySize) {
		Element e;

		if (!(e.model = Utils::LoadAsset(filename.c_str())))
			return false;
		e.list = Utils::GenerateList(e.model);

		e.size.Set(xSize, ySize);
		elements.push_back(e);
	}

	file.close();
	return true;
}

bool Level::GenerateBlocks() {
	for (int i = 1; i <= N_BLOCKS; i++) {
		std::stringstream filename;
		filename << "Levels\\level" << i << ".txt";

		std::ifstream file(filename.str(), std::ios::in);

		if (!file) {
			Utils::Log("Couldn't load level file");
			return false;
		}

		std::list<Collectible> collectibles;
		std::list<Obstacle> obstacles;
		int e, xPos, yPos;

		while (file >> e >> xPos >> yPos) {
			if (e < 2)
				collectibles.push_back(Collectible(elements[e].list, e + 1, aiVector2D(xPos, yPos), elements[e].size));
			else
				obstacles.push_back(Obstacle(elements[e].list, aiVector2D(xPos, yPos), elements[e].size));
		}

		file.close();
		blockArray.push_back(Block(wallList, obstacles, collectibles));
	}

	return true;
}

void Level::Start() {
	Block b(wallList);
	srand((unsigned)time(NULL));
	blockQueue.clear();

	b.Move(START_POS);
	blockQueue.push_back(b);
	b.Move(LENGTH);
	blockQueue.push_back(b);
}

void Level::Move(float amount) {

	for (std::list<Block>::iterator i = blockQueue.begin(); i != blockQueue.end(); ++i)
		i->Move(amount);

	if (blockQueue.front().position.x < END_POS)
		NextBlock();
}

void Level::Draw() {
	glPushMatrix();
	glTranslatef(-10.2f, -6.f, -10.f);
	glScalef(1.2f, 1.2f, 0.f);
	glCallList(backgroundList);
	glPopMatrix();

	for (std::list<Block>::iterator i = blockQueue.begin(); i != blockQueue.end(); ++i)
		i->Draw();
}

void Level::NextBlock() {
	//int n = rand() % blockArray.size();
	test = (test < N_BLOCKS - 1) ? test + 1 : 0;

	//Block b = blockArray[n];
	Block b = blockArray[test];
	b.Move(blockQueue.back().position.x + LENGTH);

	blockQueue.push_back(b);
	blockQueue.pop_front();
	std::stringstream s;
	s << test;
	Utils::Log(s.str());
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
