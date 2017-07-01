#include "Player.h"
//#include "Ground.h"
#include "aiVector3Dplus.h"

#include <stdio.h>
#include <cmath>
#include <GL/glut.h>

Player::Player(float z_pos, float z_dir){
/*	lx = 0.0f;
	lz = z_dir;	//-1.0f
	ly = 0.0f;*/
	
/*	x = 0.0f;
	z = z_pos; //0.5f
	y = 0.5;*/
	
	position.x = 0.0f;
	position.y = 0.5f;
	position.z = z_pos;
	
	mv.x = 0.0f;
	mv.y = 0.0f;
	mv.z = z_dir;
	
	v1.x = 0.0f;
	v1.y = 0.0f;
	v1.z = 0.0f;
	v2.x = 0.0f;
	v2.y = 0.0f;
	v2.z = 0.0f;
}

Player::~Player(){

}

void Player::initPosition(Road& road){
	currentFace = 0;	//all'inzio il giocatore si trova sulla prima casella di strada
	currentTile = 0;
	v1 = road.tiles[currentTile].vertices[currentFace][0];
	v2 = road.tiles[currentTile].vertices[currentFace][1];
	printf("PLAYER.initPosition(): v1: x = %.2f y = %.2f z = %.2f\nv2: x = %.2f y = %.2f z = %.2f\n", v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
}

void Player::setCamera(){
	gluLookAt(position.x/*+15.0f*/, 2.0f+mv.y, position.z+5/*mv.z*/, position.x, 1.0f+mv.y,  position.z, 0.0f, 1.0f,  0.0f);
}

void Player::computePos(float deltaMove, Road& road) {
	float    sb = 0.0f , sn = 0.0f, sd = 0.0f;
	aiVector3D n(0.0, 0.0, 0.0);
	aiVector3D proj(position.x, 0.0, position.z);
	
	position.x += deltaMove * mv.x * 0.1f;
	position.z += deltaMove * mv.z * 0.1f;
	
	do{
		n = normal(position, v1, v2);
		printf("v1: x =%.2f y=%.2f z=%.2f\nv2: x =%.2f y=%.2f z=%.2f\n", v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
		
		if(n.y < 0.001f){
			printf("normal is negative - n=%.5f  ", n.y);
			if(currentFace == road.tiles[currentTile].dim){	//sono sull'ultima faccia del pezzo di strada corrente, quindi devo incrementare curTile e reinizializzare gli altri indici
				currentFace = 0;
				currentTile++;
			}else{
				currentFace++;	//sono ancora all'interno del pezzo di strada corrente
			}
			v1 = road.tiles[currentTile].vertices[currentFace][0];
			v2 = road.tiles[currentTile].vertices[currentFace][1];
		}
		printf("currentFace: %u\n", currentFace);
	}while(n.y < 0.001f);
	
	assert(position.y < 10.0f);
	
	sn = dot(road.tiles[currentTile].normals[currentFace],(proj - v1));
	sd = dot(road.tiles[currentTile].normals[currentFace], road.tiles[currentTile].normals[currentFace]);
	sb = sn / sd;
	

	aiVector3D result = proj + sb * road.tiles[currentTile].normals[currentFace];
	mv.y = sqrt(dot((proj-result),(proj-result)));
	
	assert(mv.y < 30.0f);
	printf("POSITION: x=%.2f y=%.2f z=%.2f  n=%.5f currentFace: %u\n\n", position.x, mv.y, position.z, n.y, currentFace);	
}

void Player::computeDir(float deltaAngle) {
	//la direzione è lungo un cerchio unitario, quindi, per evitare che
	//la teiera giri attorno all'asse del cerchio, limito la virata tra
	//gli angoli -1 e 1 (cioè tra 0 e 180). L'else è per evitare che la
	//teiera continui a ruotare su se stessa quando l'angolo è oltre -1
	//o 1
	playerAngle += deltaAngle;
	if(playerAngle >= -1.0 && playerAngle <= 1.0){
		mv.x = sin(playerAngle);
		mv.z = -cos(playerAngle);
		printf("angle: %f\n", playerAngle);
	}else{
		playerAngle -= deltaAngle;
	}
}

void Player::drawPlayer(Road& road){
	//posiziona il personaggio nella scena
	if (deltaMove){
		computePos(deltaMove, road);
		//printf("positionTeapot: %f \n", deltaMove);
	}
	if (deltaAngle){
		computeDir(deltaAngle);
		printf("rotateTeapot: %f \n", -60*playerAngle+90);
	}
	
	// Set the camera
	setCamera();	
		
	//Teiera
	glPushMatrix();
	//nel rendering: prima ruota sull'origine e poi trasla
	glTranslatef(position.x+mv.x, position.y+mv.y, position.z+mv.z-0.15);	//-0.15 è per centrare la teiera rispetto a zpos
	//60* è un coefficiente per dare rotazione visibile alla teiera
	glRotatef(-60*playerAngle+90, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel (GL_SMOOTH);
	glColor3f(0.8, 0.8, 0.1);
	glutSolidTeapot(0.25);
	glPopMatrix();
}

//deltaAngle e' la direzione destra o sinistra rispetto alla posizione corrente
void Player::setDeltaAngle(float deltaAngle){
	this->deltaAngle = deltaAngle;
}

//deltaMove e' lo spostamento in avanti o all'indietro rispetto alla pos corrente 
void Player::setDeltaMove(float deltaMove){
	this->deltaMove = deltaMove;
}

//player's angle e' la rotazione del personaggio
void Player::setPlayerAngle(float playerAngle){
	this->playerAngle = playerAngle;
}

float Player::getDeltaAngle(){
	return deltaAngle;
}

float Player::getDeltaMove(){
	return deltaMove;
}

float Player::getPlayerAngle(){
	return playerAngle;
}

