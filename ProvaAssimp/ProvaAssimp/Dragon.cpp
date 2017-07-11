#include "Dragon.h"

Dragon::Dragon() {
}

bool Dragon::Init() {

	model = Utils::LoadAsset("Models\\drago.obj");
	if (!model) {
		fprintf(stderr, "Couldn't load asset");
		return false;
	};

	headList = glGenLists(1);
	glNewList(headList, GL_COMPILE);
	Utils::RecursiveRender(model, model->mRootNode->mChildren[2], 1.0);
	glEndList();

	centerList = glGenLists(1);
	glNewList(centerList, GL_COMPILE);
	Utils::RecursiveRender(model, model->mRootNode->mChildren[1], 1.0);
	glEndList();

	tailList = glGenLists(1);
	glNewList(tailList, GL_COMPILE);
	Utils::RecursiveRender(model, model->mRootNode->mChildren[0], 1.0);
	glEndList();

	parts[0].position.Set(START_XPOS, START_YPOS);
	parts[0].head.Set(START_XPOS + 2.f, START_YPOS);
	parts[0].tail.Set(START_XPOS - 2.f, START_YPOS);
	parts[0].rotation = 0.f;

	for (int i = 1; i < MAX_PARTS; i++) {
		parts[i].position.Set(parts[i - 1].tail.x - 1.f, START_YPOS);
		parts[i].head.Set(parts[i].position.x + 1.f, START_YPOS);
		parts[i].tail.Set(parts[i].position.x - 1.f, START_YPOS);
	}

	return true;
}

void Dragon::Move(bool upwards, bool downwards, float delta) {
	for (int i = 1; i <= lives + 1; i++) {

		parts[i].rotation += (parts[i - 1].rotation - parts[i].rotation) * TURN_HERITAGE;
		parts[i].position.y += (parts[i - 1].position.y - parts[i].position.y) * MOVEMENT_HERITAGE;

		float xDelta = cos(parts[i].rotation * PI / 180.0);
		float yDelta = sin(parts[i].rotation * PI / 180.0);

		parts[i].head.x = parts[i].position.x + xDelta;
		parts[i].head.y = parts[i].position.y + yDelta;
		parts[i].tail.x = parts[i].position.x - xDelta;
		parts[i].tail.y = parts[i].position.y - yDelta;

		xDelta = parts[i - 1].tail.x - parts[i].head.x;
		yDelta = parts[i - 1].tail.y - parts[i].head.y;

		parts[i].head.x += xDelta;
		parts[i].head.y += yDelta;
		parts[i].position.x += xDelta;
		parts[i].position.y += yDelta;
		parts[i].tail.x += xDelta;
		parts[i].tail.y += yDelta;
	}

	if (upwards) {
		movVel = Utils::Clamp(MOV_ACEL * delta, MAX_MOV_VEL, movVel + MOV_ACEL * delta);
		parts[0].rotation = Utils::Clamp(MIN_ROT, MAX_ROT, parts[0].rotation + ROT_VEL * 5.0 * delta);
		
	} else if (downwards) {
		movVel = Utils::Clamp(MIN_MOV_VEL, -MOV_ACEL * delta, movVel - MOV_ACEL * delta);
		parts[0].rotation = Utils::Clamp(MIN_ROT, MAX_ROT, parts[0].rotation - ROT_VEL * 5.0 * delta);	

	} else if (parts[0].rotation < 0.f || movVel < 0.f) {
		movVel = Utils::Clamp(MIN_MOV_VEL, 0.f, movVel + MOV_ACEL * delta);
		parts[0].rotation = Utils::Clamp(MIN_ROT, 0.f, parts[0].rotation + ROT_VEL * delta);

	} else if (parts[0].rotation > 0.f || movVel > 0.f) {
		movVel = Utils::Clamp(0.f, MAX_MOV_VEL, movVel - MOV_ACEL * delta);
		parts[0].rotation = Utils::Clamp(0.f, MAX_ROT, parts[0].rotation - ROT_VEL * delta);
	}

	parts[0].position.y = Utils::Clamp(YMIN, YMAX, parts[0].position.y + movVel * delta);

	float xDelta = 2.f * cos(parts[0].rotation * PI / 180.0);
	float yDelta = 2.f * sin(parts[0].rotation * PI / 180.0);

	parts[0].head.x = parts[0].position.x + xDelta;
	parts[0].head.y = parts[0].position.y + yDelta;
	parts[0].tail.x = parts[0].position.x - xDelta;
	parts[0].tail.y = parts[0].position.y - yDelta;

}

void Dragon::Draw() {
	glPushMatrix();
	glTranslatef(parts[0].position.x, parts[0].position.y, 0.f);
	glRotatef(parts[0].rotation, 0.f, 0.f, 1.f);
	glCallList(headList);
	glPopMatrix();

	int i;
	for (i = 1; i <= lives; i++) {
		glPushMatrix();
		glTranslatef(parts[i].position.x, parts[i].position.y, 0.f);
		glRotatef(parts[i].rotation, 0.f, 0.f, 1.f);
		glCallList(centerList);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(parts[i].position.x, parts[i].position.y, 0.f);
	glRotatef(parts[i].rotation, 0.f, 0.f, 1.f);
	glCallList(tailList);
	glPopMatrix();
}


Dragon::~Dragon() {
	aiReleaseImport(model);
}
