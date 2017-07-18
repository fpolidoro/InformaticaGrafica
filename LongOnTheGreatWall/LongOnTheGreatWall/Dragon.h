#pragma once
#include "Utils.h"
#include <math.h>

#define PI 3.14159265f

#define MAX_NODES 7
#define MAX_LIVES MAX_NODES - 2
#define NODE_DIST 2.f
#define HITBOX_SIZE 1.5f

#define START_XPOS 10.f
#define START_YPOS 8.f

#define X_VEL 20.f
#define ROT_VEL 60.f
#define MOV_ACEL 64.f
#define MAX_MOV_VEL 24.f
#define MIN_MOV_VEL -MAX_MOV_VEL

#define MIN_Y_POS 2.f
#define MAX_Y_POS 18.f
#define MAX_ROT 30.f
#define MIN_ROT -MAX_ROT
#define MAX_ROT_RAD (MAX_ROT * PI / 180.f)
#define MIN_ROT_RAD (MIN_ROT * PI / 180.f)
#define VAR_RAD (5.f * PI / 180.f)

#define HIT_TIME 1.f
#define GRAVITY 140.f
#define JUMP_VEL 40.f

class Dragon {
public:
	Dragon();
	bool Init();
	void Start();
	void Move(bool upwards, bool downwards, float delta);
	void Draw();
	bool GainLife();
	bool LoseLife();
	aiVector2D GetBottomLeft();
	aiVector2D GetTopRight();
	~Dragon();

private:
	const struct aiScene* model;

	GLuint headList;
	GLuint centerList;
	GLuint tailList;

	struct Node {
		aiVector2D position;
		float rotation;
	};
	Node nodes[MAX_NODES];

	aiVector2D lostNode;
	bool animateLostNode;
	bool animateDeath;
	float fallTime;
	float fallVel;

	float movVel;
	int lives;
};

