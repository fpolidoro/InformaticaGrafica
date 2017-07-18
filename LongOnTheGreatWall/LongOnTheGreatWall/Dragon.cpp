#include "Dragon.h"

Dragon::Dragon() {
}

bool Dragon::Init() {

	model = Utils::LoadAsset("Models\\drago.obj");
	if (!(model = Utils::LoadAsset("Models\\drago.obj")))
		return false;

	headList = Utils::GenerateList(model, model->mRootNode->mChildren[2]);
	centerList = Utils::GenerateList(model, model->mRootNode->mChildren[1]);
	tailList = Utils::GenerateList(model, model->mRootNode->mChildren[0]);

	return true;
}

void Dragon::Start() {
	lives = MAX_LIVES;
	movVel = 0.f;
	animateLostNode = false;
	animateDeath = false;

	nodes[0].position.Set(START_XPOS, START_YPOS);
	nodes[0].rotation = 0.f;

	for (int i = 1; i < MAX_NODES; i++)
		nodes[i].position.Set(nodes[i - 1].position.x - NODE_DIST, START_YPOS);
}

void Dragon::Move(bool upwards, bool downwards, float deltaTime) {

	if (animateDeath) {
		if (fallTime < HIT_TIME) {
			fallTime += deltaTime;
			fallVel -= GRAVITY * deltaTime;
			for (int i = 0; i < 2; i++)
				nodes[i].position.y += fallVel * deltaTime;
		} else {
			animateDeath = false;
		}
	} else {
		//Nodes movement
		for (int i = 1; i <= lives + 1; i++) {
			float dx = nodes[i - 1].position.x - nodes[i].position.x + X_VEL * deltaTime;
			float dy = nodes[i - 1].position.y - nodes[i].position.y;

			float radians = Utils::Clamp(MIN_ROT_RAD - VAR_RAD * i, MAX_ROT_RAD + VAR_RAD * i, atan2f(dy, dx));
			nodes[i].rotation = radians * 180.f / PI;

			float distance = (i == 1) ? NODE_DIST + 1.f : NODE_DIST;
			nodes[i].position.x = nodes[i - 1].position.x - distance * cos(radians);
			nodes[i].position.y = nodes[i - 1].position.y - distance * sin(radians);
		}

		//Head movement
		if (upwards && nodes[0].position.y < MAX_Y_POS) {
			movVel = Utils::Clamp(MOV_ACEL * deltaTime, MAX_MOV_VEL, movVel + MOV_ACEL * deltaTime);
			nodes[0].rotation = Utils::Clamp(MIN_ROT, MAX_ROT, nodes[0].rotation + ROT_VEL * 5.0 * deltaTime);

		} else if (downwards && nodes[0].position.y > MIN_Y_POS) {
			movVel = Utils::Clamp(MIN_MOV_VEL, -MOV_ACEL * deltaTime, movVel - MOV_ACEL * deltaTime);
			nodes[0].rotation = Utils::Clamp(MIN_ROT, MAX_ROT, nodes[0].rotation - ROT_VEL * 5.0 * deltaTime);

		} else if (nodes[0].rotation < 0.f || movVel < 0.f) {
			movVel = Utils::Clamp(MIN_MOV_VEL, 0.f, movVel + MOV_ACEL * deltaTime);
			nodes[0].rotation = Utils::Clamp(MIN_ROT, 0.f, nodes[0].rotation + ROT_VEL * deltaTime);

		} else if (nodes[0].rotation > 0.f || movVel > 0.f) {
			movVel = Utils::Clamp(0.f, MAX_MOV_VEL, movVel - MOV_ACEL * deltaTime);
			nodes[0].rotation = Utils::Clamp(0.f, MAX_ROT, nodes[0].rotation - ROT_VEL * deltaTime);
		}

		nodes[0].position.y = Utils::Clamp(MIN_Y_POS, MAX_Y_POS, nodes[0].position.y + movVel * deltaTime);

		//Lost node animation
		if (animateLostNode) {
			if (fallTime < HIT_TIME) {
				fallTime += deltaTime;
				fallVel -= GRAVITY * deltaTime;
				lostNode.y += fallVel * deltaTime;
			} else {
				animateLostNode = false;
			}
		}
	}
}

void Dragon::Draw() {
	//Head
	glPushMatrix();
	glTranslatef(nodes[0].position.x, nodes[0].position.y, 0.f);
	glRotatef(nodes[0].rotation, 0.f, 0.f, 1.f);
	glCallList(headList);
	glPopMatrix();

	//Center
	int i;
	for (i = 1; i <= lives; i++) {
		glPushMatrix();
		glTranslatef(nodes[i].position.x, nodes[i].position.y, 0.f);
		glRotatef(nodes[i].rotation, 0.f, 0.f, 1.f);
		glCallList(centerList);
		glPopMatrix();
	}

	//Tail
	glPushMatrix();
	glTranslatef(nodes[i].position.x, nodes[i].position.y, 0.f);
	glRotatef(nodes[i].rotation, 0.f, 0.f, 1.f);
	glCallList(tailList);
	glPopMatrix();

	//Lost node animation
	if (animateLostNode) {
		glPushMatrix();
		glTranslatef(lostNode.x, lostNode.y, 0.f);
		glCallList(centerList);
		glPopMatrix();
	}

}

bool Dragon::GainLife() {
	if (lives < MAX_LIVES) {
		lives++;
		return true;
	} else {
		return false;
	}
}

bool Dragon::LoseLife() {
	if (lives > 0) {
		lives--;
		animateLostNode = true;
		lostNode.Set(nodes[1].position.x, nodes[1].position.y);
		fallVel = JUMP_VEL;
		fallTime = 0.f;
		return true;
	}
	animateDeath = true;
	fallVel = JUMP_VEL;
	fallTime = 0.f;
	return false;
}

aiVector2D Dragon::GetBottomLeft() {
	return nodes[0].position - HITBOX_SIZE;
}

aiVector2D Dragon::GetTopRight() {
	return nodes[0].position + HITBOX_SIZE;
}

Dragon::~Dragon() {
	aiReleaseImport(model);
}
