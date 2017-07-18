#pragma once
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"

#include <GL\glut.h>
#include <IL\il.h>

#include <string.h>
#include <map>
#include <iostream>

static class Utils {
public:
	static void GetBoundingBoxForNode(const struct aiScene* sc, const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo);
	static void GetBoundingBox(const struct aiScene* sc, struct aiVector3D* min, struct aiVector3D* max);

	static void Color2Float(const struct aiColor4D *c, float f[4]);
	static void Color4f(const struct aiColor4D *color);
	static void SetFloat4(float f[4], float a, float b, float c, float d);
	static float Clamp(float min, float max, float value);
	static float Lerp(float a, float b, float t);

	static void ApplyMaterial(const struct aiMaterial *mtl);
	static void RecursiveRender(const struct aiScene *sc, const struct aiNode* nd, float scale);

	static const aiScene* LoadAsset(const char* path);
	static const aiScene* LoadAssetWithBoundingBox(const char* path, struct aiVector3D* min, struct aiVector3D* max);

	static bool LoadGLTextures(const aiScene* scene);

	static void AttachStream();
	static void DetachStream();

	static void Log(std::string string);
	static GLuint GenerateList(const aiScene* scene, const aiNode* node);

private:
	static const std::string basepath;

	static std::map<std::string, GLuint*> textureIdMap;
	static GLuint* textureIds;

	static struct aiLogStream stream;
};

