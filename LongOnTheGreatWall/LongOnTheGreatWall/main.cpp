#include "Level.h"
#include "Dragon.h"
#include "irrKlang.h"

using namespace irrklang;

GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat lightPosition[] = { 15.f, 30.f, 15.0f, 1.0f };

Level level;
Dragon dragon;
int oldTime = 0.f, score = 0.f, highScore = 0.f;
bool turnUp = false, turnDown = false, enter = false, inGame = false, invulnerable = false, dead = false;
float hitTimer = 0.f;

const aiScene* menu;
GLuint menuList;

ISoundEngine* engine;

#define FIELD_OF_VIEW 45.f
#define Z_NEAR 1.f
#define Z_FAR 200.f
#define HIT_TIME 1.f
#define ASPECT_RATIO (16.f / 9.f)

void Reshape(int width, int height) {
	int newWidth = height * ASPECT_RATIO;
	int newHeight = width / ASPECT_RATIO;

	if (newWidth > width) newWidth = width;
	if (newHeight > height) newHeight = height;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(FIELD_OF_VIEW, ASPECT_RATIO, Z_NEAR, Z_FAR);
	glViewport(0, 0, newWidth, newHeight);
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
		else if (!((int)(hitTimer * 10.f) % 2))
			dragon.Draw();
	}

	glutSwapBuffers();
}

void Idle(void) {
	if (!inGame) {
		if (enter) {
			oldTime = glutGet(GLUT_ELAPSED_TIME);
			score = 0.f;
			hitTimer = 0.f;
			inGame = true;
			invulnerable = false;
			dead = false;

			level.Start();
			dragon.Start();
			engine->stopAllSounds();
			engine->play2D("Sound\\game.wav", true);
		}
		glutPostRedisplay();

	} else {
		int time;
		float deltaTime;
		time = glutGet(GLUT_ELAPSED_TIME);
		deltaTime = (time - oldTime) / 1000.0;	// In seconds
		oldTime = time;

		dragon.Move(turnUp, turnDown, deltaTime);

		if (!dead) {
			level.Move(deltaTime);

			if (!invulnerable) {
				switch (level.CheckHit(dragon.GetBottomLeft(), dragon.GetTopRight())) {
				//Coin
				case 1:
					score += 100;
					highScore = (score > highScore) ? score : highScore;
					engine->play2D("Sound\\coin.wav");
					break;
				//Life
				case 2:
					if (dragon.GainLife()) {
						engine->play2D("Sound\\life.wav");
					} else {
						score += 500;
						engine->play2D("Sound\\coin.wav");
					}
					break;
				//Obstacle
				case 3:
					if (dragon.LoseLife()) {
						invulnerable = true;
					} else {
						dead = true;
					}
					hitTimer = HIT_TIME;
					engine->play2D("Sound\\fail.wav");
					break;

				default:
					break;
				}

			} else {
				hitTimer -= deltaTime;
				if (hitTimer <= 0.f) {
					hitTimer = 0.f;
					invulnerable = false;
				}
			}

		} else {
			hitTimer -= deltaTime;
			if (hitTimer <= 0.f) {
				inGame = false;
				engine->stopAllSounds();
				engine->play2D("Sound\\menu.wav", true);
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

void SpecialKeyDown(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		turnUp = true;
		turnDown = false;
		break;
	case GLUT_KEY_DOWN:
		turnUp = false;
		turnDown = true;
		break;
	default:
		break;
	}
}


void SpecialKeyUp(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		turnUp = false;
		break;
	case GLUT_KEY_DOWN:
		turnDown = false;
		break;
	default:
		break;
	}
}


bool InitGL() {
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);				
	glEnable(GL_DEPTH_TEST);		
	glDepthFunc(GL_LEQUAL);			
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);

	glEnable(GL_LIGHT1);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;					
}

bool InitGame() {
	if (!(menu = Utils::LoadAsset("Models\\menu.obj")))
		return false;
	menuList = Utils::GenerateList(menu, menu->mRootNode);

	if (!dragon.Init() || !level.Init())
		return false;

	engine = createIrrKlangDevice();
	if (!engine) {
		printf("Could not startup engine\n");
		return false;
	}

	engine->play2D("Sound\\menu.wav", true);

	return true;
}

int main(int argc, char **argv) {
	struct aiLogStream stream;

	glutInitWindowSize(1600, 900);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutCreateWindow("Long on the great wall");
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutSpecialFunc(SpecialKeyDown);
	glutSpecialUpFunc(SpecialKeyUp);

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

	engine->drop();

	//Utils::DetachStream();

	return 0;
}
