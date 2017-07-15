#pragma once
#include "Utils.h"

#define PI 3.14159265f

#define MAX_PARTS 7
#define MAX_LIVES MAX_PARTS - 2
#define HITBOX_SIZE 1.5f

#define START_XPOS 10.f
#define START_YPOS 8.f

#define ROT_VEL 60.f
#define MOV_ACEL 64.f
#define MAX_MOV_VEL 24.f
#define MIN_MOV_VEL -MAX_MOV_VEL

#define MOVEMENT_HERITAGE 80.f / 100.f
#define ROT_HERITAGE 5.f / 100.f

#define YMIN 2.f
#define YMAX 18.f
#define MAX_ROT 30.f
#define MIN_ROT -MAX_ROT

class Dragon {
public:
	Dragon();
	bool Init();
	void Start();
	void Move(bool upwards, bool downwards, float delta);
	void Draw();
	void GainLife();
	bool LoseLife();
	aiVector2D GetBottomLeft();
	aiVector2D GetTopRight();
	~Dragon();

private:
	const struct aiScene* model;

	GLuint headList;
	GLuint centerList;
	GLuint tailList;

	struct Part {
		aiVector2D position;
		aiVector2D head;
		aiVector2D tail;
		float rotation;
	};
	Part parts[MAX_PARTS];

	float movVel;
	int lives;

};

