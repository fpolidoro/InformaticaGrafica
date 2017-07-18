#include "Level.h"

bool Level::Init() {
	if (!LoadAssets())
		return false;

	if (!GenerateBlocks())
		return false;

	return true;
}

bool Level::LoadAssets() {

	if (!(background = Utils::LoadAsset("Models\\sfondo.obj")))
		return false;
	skyList = Utils::GenerateList(background, background->mRootNode->mChildren[0]);
	bgList[0] = Utils::GenerateList(background, background->mRootNode->mChildren[1]);
	bgList[1] = Utils::GenerateList(background, background->mRootNode->mChildren[2]);
	bgList[2] = Utils::GenerateList(background, background->mRootNode->mChildren[4]);
	bgList[3] = Utils::GenerateList(background, background->mRootNode->mChildren[3]);

	if (!(wall = Utils::LoadAsset("Models\\muro.obj")))
		return false;
	wallList = Utils::GenerateList(wall, wall->mRootNode);

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
		e.list = Utils::GenerateList(e.model, e.model->mRootNode);

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

	for (int i = 0; i < N_BG; i++)
		bgArray[i] = BG_X_POS;

	velocity = VELOCITY;

	//test = 6;
}

void Level::Move(float deltaTime) {

	for (std::list<Block>::iterator i = blockQueue.begin(); i != blockQueue.end(); ++i)
		i->Move(-velocity * deltaTime);

	if (blockQueue.front().position.x < END_POS)
		NextBlock();

	for (int i = 0; i < N_BG; i++) {
		bgArray[i] -= velocity / (BG_VELOCITY_FACTOR * (N_BG - i)) * deltaTime;
		if (bgArray[i] < BG_X_POS - BG_LENGTH) {
			bgArray[i] += BG_LENGTH;
		}
	}
}

void Level::Draw() {
	//Sky
	glPushMatrix();
	glTranslatef(BG_X_POS, BG_Y_POS, BG_Z_POS);
	glScalef(BG_SCALE, BG_SCALE, 0.f);
	glCallList(skyList);
	glPopMatrix();

	//Background levels
	for (int i = 0; i < 4; i++) {
		glPushMatrix();
		glTranslatef(bgArray[i], BG_Y_POS, BG_Z_POS + 0.1f * i);
		glScalef(BG_SCALE, BG_SCALE, 0.f);
		glCallList(bgList[i]);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(bgArray[i] + BG_LENGTH, BG_Y_POS, BG_Z_POS + 0.1f * i);
		glScalef(BG_SCALE, BG_SCALE, 0.f);
		glCallList(bgList[i]);
		glPopMatrix();
	}

	//Block
	for (std::list<Block>::iterator i = blockQueue.begin(); i != blockQueue.end(); ++i)
		i->Draw();
}

void Level::NextBlock() {
	int n = rand() % blockArray.size();
	Block b = blockArray[n];
	if (blockCount < N_BLOCKS_TO_FASTER) {
		blockCount++;
	} else {
		blockCount = 0;
		velocity += VELOCITY_INCREASE;
	}

	//test = (test < N_BLOCKS - 1) ? test + 1 : 0;
	//Block b = blockArray[test];

	b.Move(blockQueue.back().position.x + LENGTH);
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
	aiReleaseImport(background);

	for (int i = 0; i < elements.size(); i++)
		aiReleaseImport(elements[i].model);
}
