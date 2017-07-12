#pragma once
#include "Utils.h"

#define PI 3.14159265f

#define MAX_PARTS 8
#define MAX_LIVES MAX_PARTS - 2

#define START_XPOS 10.f
#define START_YPOS 8.f

#define ROT_VEL 60.f
#define MOV_ACEL 64.f
#define MAX_MOV_VEL 24.f
#define MIN_MOV_VEL -MAX_MOV_VEL

#define MOVEMENT_HERITAGE 60.f / 100.f
#define ROT_HERITAGE 4.f / 100.f

#define YMIN 2.f
#define YMAX 18.f
#define MAX_ROT 30.f
#define MIN_ROT -MAX_ROT

class Dragon {
public:
	Dragon();
	bool Init();
	void Move(bool upwards, bool downwards, float delta);
	void Draw();
	void GainLife();
	void LoseLife();
	aiVector2D GetBottomLeft();
	aiVector2D GetTopRight();
	~Dragon();

private:
	const struct aiScene* model = NULL;

	GLuint headList = 0;
	GLuint centerList = 0;
	GLuint tailList = 0;

	struct Part {
		aiVector2D position;
		aiVector2D head;
		aiVector2D tail;
		float rotation;
	};
	Part parts[MAX_PARTS];

	float movVel = 0.f;
	int lives = MAX_LIVES;

};

