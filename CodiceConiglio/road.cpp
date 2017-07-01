#include "assimp/scene.h"
#include "assimp/vector3.h"
#include "aiVector3Dplus.h"
#include "road.h"

Road::Road(){
}

Road::~Road(){
	road = NULL;
	
	//ricontrollare con valgrind
	for(unsigned int i=0; i<totTiles; i++){
		free(tiles[i].normals);
		for(unsigned int j=0; j<tiles[i].dim; j++){
			free(tiles[i].vertices[j]);
		}
		free(tiles);
	}
	
	free(tiles);
}

unsigned int Road::initRoad(const struct aiNode *nd, const unsigned int& numFaces){
	road = nd;
	assert(numFaces != 0);
	if(tiles == NULL){	//sono alla prima allocazione della strada
		currAllocIndex = 0;
		tiles = (tile_t *)malloc(sizeof(tile_t));	//alloco l'array di pezzi di strada (tiles)
		assert(tiles != NULL);
	}else{	//ho gia' allocato dei pezzi di strada, alloco i successivi
		currAllocIndex++;
		tiles = (tile_t *)realloc(tiles, (currAllocIndex+1)*sizeof(tile_t));
		assert(tiles != NULL);
	}
	
	tiles[currAllocIndex].vertices = (aiVector3D **)malloc(numFaces*sizeof(aiVector3D *));
	tiles[currAllocIndex].dim = numFaces;
	assert(tiles[currAllocIndex].vertices != NULL);
	
	for(unsigned int i=0; i<numFaces; i++){
		tiles[currAllocIndex].vertices[i] = (aiVector3D *)malloc(2*sizeof(aiVector3D));	//due blocchetti a riga per salvare i due vertici superiori del triangolo
		assert(tiles[currAllocIndex].vertices[i] != NULL);

		for(unsigned int j=0; j<2; j++){
			tiles[currAllocIndex].vertices[i][j].x = 0.0f;
			tiles[currAllocIndex].vertices[i][j].y = 0.0f;
			tiles[currAllocIndex].vertices[i][j].z = 0.0f;
		}
	}
	
	tiles[currAllocIndex].normals = (aiVector3D *)malloc(numFaces*sizeof(aiVector3D));
	assert(tiles[currAllocIndex].normals != NULL);
	
	return currAllocIndex;
}

/*
unsigned int Road::getNofFaces(){
	return 0;	//todo
}*/
