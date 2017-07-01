#ifndef PLAYER_H
#define PLAYER_H

#include "road.h"

class Player{
	public:
		Player(float z_pos, float z_dir);
		virtual ~Player();
		void setDeltaAngle(float deltaAngle);
		void setDeltaMove(float deltaMove);
		void setPlayerAngle(float playerAngle);
		float getDeltaAngle();
		float getDeltaMove();
		float getPlayerAngle();
		void setCamera();
		void drawPlayer(Road& road);
		void initPosition(Road& road);
	
	private:
		float deltaAngle;
		float deltaMove;
		float playerAngle;
		//float x, y, z, lx, ly, lz;
		aiVector3D position;
		aiVector3D mv;
		aiVector3D v1, v2;	//vertici superiori della faccia corrente
		unsigned int currentFace;	//faccia corrente del pavimento
		unsigned int currentTile; 	//pezzo di strada corrente
						
		void computeDir(float deltaAngle);
		void computePos(float deltaMove, Road& road);
	
};

#endif
