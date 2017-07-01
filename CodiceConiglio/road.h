#ifndef ROAD_H
#define ROAD_H

#include "assimp/scene.h"
#include "assimp/vector3.h"

typedef struct{
	aiVector3D **vertices;	//contiene i 2 vertici superiori di ciascuna faccia
	aiVector3D *normals;	//contiene le normali di ciascuna faccia
	unsigned int dim;		//dimensione dei due array
}tile_t;

class Road{
	public:
		Road();
		~Road();
		unsigned int initRoad(const struct aiNode *nd, const unsigned int& numFaces);
		//unsigned int getNofFaces();
	
		tile_t *tiles;	//contiene i pezzi di strada
		
	private:
		const aiNode *road;
		unsigned int totTiles; //numero totale dei pezzi di strada (tiles), dimensione dell'array tiles
		unsigned int currAllocIndex;	//indice temporaneo per l'allocazione dei pezzi di strada
};

#endif
