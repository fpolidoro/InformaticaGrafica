#include "Level.h"
#include "Dragon.h"

GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 0.0f, 15.f, 15.0f, 1.0f };

Level level;
Dragon dragon;
int oldTime = 0.f, score = 0.f, highScore = 0.f;
bool turnUp = false, turnDown = false, enter = false, inGame = false, invulnerable = false;
float hitTimer = 0.f;

const aiScene* menu;
GLuint menuList;

#define FIELD_OF_VIEW 45.f
#define Z_NEAR 1.f
#define Z_FAR 200.f
#define VELOCITY 12.f
#define HIT_TIME 1.f

void Reshape(int width, int height) {
	const double aspectRatio = (float)width / height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspectRatio, Z_NEAR, Z_FAR);
	glViewport(0, 0, width, height);
}

void DrawScore() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(14.f, 17.5f, 2.f);
	glScalef(.005f, .005f, .005f);
	glColor3f(0.f, 0.f, 0.f);
	glRasterPos2f(0.f, 0.f);

	std::stringstream scoreString;
	scoreString << "High Score: " << highScore;
	for (int i = 0; i < scoreString.str().length(); i++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, scoreString.str()[i]);

	glPopMatrix();

	if (inGame) {
		glPushMatrix();
		glTranslatef(-0.5f, 17.5f, 2.f);
		glScalef(.005f, .005f, .005f);
		glColor3f(0.f, 0.f, 0.f);
		glRasterPos2f(0.f, 0.f);

		scoreString.str("");
		scoreString.clear();
		scoreString << "Score: " << score;
		for (int i = 0; i < scoreString.str().length(); i++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, scoreString.str()[i]);

		glPopMatrix();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(16.f, 8.6f, 25.5f, 16.f, 8.6f, 0.f, 0.f, 1.f, 0.f);

	DrawScore();

	if (!inGame) {
		glPushMatrix();
		glTranslatef(-10.2f, -6.1f, -10.f);
		glScalef(2.3f, 2.3f, 0.f);
		glCallList(menuList);
		glPopMatrix();
	} else {
		level.Draw();

		if (!invulnerable)
			dragon.Draw();
		else if ((int)(hitTimer * 10.f) % 2)
			dragon.Draw();
	}

	glutSwapBuffers();
}

void Idle(void) {
	if (!inGame) {
		if (enter) {
			inGame = true;
			oldTime = glutGet(GLUT_ELAPSED_TIME);
			score = 0.f;
			level.Start();
			dragon.Start();
		}
		glutPostRedisplay();
	} else {

		int time;
		float deltaTime;
		time = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = (time - oldTime) / 1000.0;	// In seconds
		oldTime = time;

		dragon.Move(turnUp, turnDown, deltaTime);
		level.Move(-VELOCITY*deltaTime);

		if (!invulnerable) {
			switch (level.CheckHit(dragon.GetBottomLeft(), dragon.GetTopRight())) {
			case 1:
				score += 100;
				highScore = (score > highScore) ? score : highScore;
				break;
			case 2:
				dragon.GainLife();
				break;
			case 3:
				if (dragon.LoseLife()) {
					hitTimer = HIT_TIME;
					invulnerable = true;
				} else {
					inGame = false;
				}
				break;
			default:
				break;
			}
		}
		else {
			hitTimer -= deltaTime;
			if (hitTimer <= 0.f) {
				hitTimer = 0.f;
				invulnerable = false;
			}
		}

		glutPostRedisplay();
	}
}

void KeyDown(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(1);
		break;
	case 'w':
	case 'W':
		turnUp = true;
		turnDown = false;
		break;
	case 's':
	case 'S':
		turnUp = false;
		turnDown = true;
		break;
	case 13:
		enter = true;
		break;
	default:
		break;
	}
}

void KeyUp(unsigned char key, int x, int y) {
	switch (key) {
	case 27:
		exit(1);
		break;
	case 'w':
	case 'W':
		turnUp = false;
		break;
	case 's':
	case 'S':
		turnDown = false;
		break;
	case 13:
		enter = false;
		break;
	default:
		break;
	}
}


bool InitGL() {
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glEnable(GL_LIGHT1);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;					// Initialization Went OK
}

bool InitGame() {
	if (!(menu = Utils::LoadAsset("Models\\menu.obj")))	
		return false;
	menuList = Utils::GenerateList(menu);

	if (!dragon.Init() || !level.Init())
		return false;

	/*oldTime = glutGet(GLUT_ELAPSED_TIME);*/
	return true;
}

int main(int argc, char **argv) {
	struct aiLogStream stream;

	glutInitWindowSize(1600, 900);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutCreateWindow("Prova drago");
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);

	//Utils::AttachStream();

	if (!InitGL()) {
		fprintf(stderr, "Initialization failed");
		return 1;
	}

	if (!InitGame()) {
		fprintf(stderr, "Initialization failed");
		return 1;
	}

	glutMainLoop();

	//Utils::DetachStream();

	return 0;
}
