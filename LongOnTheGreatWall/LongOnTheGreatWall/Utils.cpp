#include "Utils.h"

const std::string Utils::basepath = "Models\\";

std::map<std::string, GLuint*> Utils::textureIdMap;
GLuint* Utils::textureIds;

struct aiLogStream Utils::stream;

void Utils::GetBoundingBoxForNode(const struct aiScene* sc, const struct aiNode* nd, struct aiVector3D* min, struct aiVector3D* max, struct aiMatrix4x4* trafo) {
	struct aiMatrix4x4 prev;
	unsigned int n = 0, t;

	prev = *trafo;
	aiMultiplyMatrix4(trafo, &nd->mTransformation);

	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];
		for (t = 0; t < mesh->mNumVertices; ++t) {

			struct aiVector3D tmp = mesh->mVertices[t];
			aiTransformVecByMatrix4(&tmp, trafo);

			min->x = std::min(min->x, tmp.x);
			min->y = std::min(min->y, tmp.y);
			min->z = std::min(min->z, tmp.z);

			max->x = std::max(max->x, tmp.x);
			max->y = std::max(max->y, tmp.y);
			max->z = std::max(max->z, tmp.z);
		}
	}

	for (n = 0; n < nd->mNumChildren; ++n) {
		GetBoundingBoxForNode(sc, nd->mChildren[n], min, max, trafo);
	}
	*trafo = prev;
}

void Utils::GetBoundingBox(const struct aiScene* sc, struct aiVector3D* min, struct aiVector3D* max) {
	struct aiMatrix4x4 trafo;
	aiIdentityMatrix4(&trafo);

	min->x = min->y = min->z = 1e10f;
	max->x = max->y = max->z = -1e10f;
	GetBoundingBoxForNode(sc, sc->mRootNode, min, max, &trafo);
}

void Utils::Color2Float(const struct aiColor4D *c, float f[4]) {
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

void Utils::Color4f(const struct aiColor4D *color) {
	glColor4f(color->r, color->g, color->b, color->a);
}

void Utils::SetFloat4(float f[4], float a, float b, float c, float d) {
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}

float Utils::Clamp(float min, float max, float value) {
	if (value < min) return min;
	else if (value > max) return max;
	else return value;
}

float Utils::Lerp(float a, float b, float t) {
	return (1 - t)*a + t*b;
}

void Utils::ApplyMaterial(const struct aiMaterial *mtl) {
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	struct aiColor4D diffuse;
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	int max;

	int texIndex = 0;
	aiString texPath;	//contains filename of texture
	if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)) {
		//bind texture
		unsigned int texId = *textureIdMap[texPath.data];
		glBindTexture(GL_TEXTURE_2D, texId);
	}

	SetFloat4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		Color2Float(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	SetFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		Color2Float(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	SetFloat4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		Color2Float(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	SetFloat4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		Color2Float(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, (unsigned int *)&max);
	max = 1;
	ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, (unsigned int *)&max);
	if ((ret1 == AI_SUCCESS) && (ret2 == AI_SUCCESS))
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		SetFloat4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, (unsigned int *)&max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, (unsigned int *)&max)) && two_sided)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void Utils::RecursiveRender(const struct aiScene *sc, const struct aiNode* nd, float scale) {
	unsigned int i;
	unsigned int n = 0, t;
	struct aiMatrix4x4 m = nd->mTransformation;

	printf("Node name: %s\n", nd->mName.data);

	//m.Scaling(aiVector3D(scale, scale, scale), m);

	// update transform
	m.Transpose();
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		///
		printf("Drawing MESH with this name: %s\n", mesh->mName.data);

		ApplyMaterial(sc->mMaterials[mesh->mMaterialIndex]);


		if (mesh->HasTextureCoords(0))
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		if (mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		}
		else {
			glEnable(GL_LIGHTING);
		}

		if (mesh->mColors[0] != NULL) {
			glEnable(GL_COLOR_MATERIAL);
		}
		else {
			glDisable(GL_COLOR_MATERIAL);
		}


		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch (face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for (i = 0; i < face->mNumIndices; i++)		// go through all vertices in face
			{
				int vertexIndex = face->mIndices[i];	// get group index for current index
				if (mesh->mColors[0] != NULL)
					Color4f(&mesh->mColors[0][vertexIndex]);
				if (mesh->mNormals != NULL)

					if (mesh->HasTextureCoords(0))		//HasTextureCoords(texture_coordinates_set)
					{
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]
					}

				glNormal3fv(&mesh->mNormals[vertexIndex].x);
				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}

			glEnd();

		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		RecursiveRender(sc, nd->mChildren[n], scale);
	}

	glPopMatrix();
}

const aiScene* Utils::LoadAssetWithBoundingBox(const char* path, struct aiVector3D* min, struct aiVector3D* max) {
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_Quality);

	if (!scene) return NULL;
	if (!LoadGLTextures(scene)) return NULL;

	GetBoundingBox(scene, min, max);
	return scene;

}

const aiScene* Utils::LoadAsset(const char* path) {
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_Quality);

	if (!scene) {
		Utils::Log("Couldn't load asset");
		return NULL;
	}
	if (!LoadGLTextures(scene)) {
		Utils::Log("Couldn't load texture");
		return NULL;
	}

	return scene;
}

bool Utils::LoadGLTextures(const aiScene* scene) {
	ILboolean success;

	/* Before calling ilInit() version should be checked. */
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION) {
		ILint test = ilGetInteger(IL_VERSION_NUM);
		/// wrong DevIL version ///
		std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
		char* cErr_msg = (char *)err_msg.c_str();

		return false;
	}

	ilInit(); /* Initialization of DevIL */

			  //if (scene->HasTextures()) abortGLInit("Support for meshes with embedded textures is not implemented");

			  /* getTexture Filenames and Numb of Textures */
	for (unsigned int m = 0; m < scene->mNumMaterials; m++) {
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS) {
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			textureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	int numTextures = textureIdMap.size();

	/* array with DevIL image IDs */
	ILuint* imageIds = NULL;
	imageIds = new ILuint[numTextures];

	/* generate DevIL Image IDs */
	ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */

										/* create and fill array with GL texture ids */
	textureIds = new GLuint[numTextures];
	glGenTextures(numTextures, textureIds); /* Texture name generation */

											/* define texture path */
											//std::string texturepath = "../../../test/models/Obj/";

											/* get iterator */
	std::map<std::string, GLuint*>::iterator itr = textureIdMap.begin();

	for (int i = 0; i < numTextures; i++) {

		//save IL image ID
		std::string filename = (*itr).first;  // get filename
		(*itr).second = &textureIds[i];	  // save texture id for filename in map
		itr++;								  // next texture


		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
		std::string fileloc = basepath + filename;	/* Loading of image */
		success = ilLoadImage((const wchar_t *)fileloc.c_str());

		fprintf(stdout, "Loading Image: %s\n", fileloc.data());

		if (success) /* If no error occured: */
		{
			success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE); /* Convert every colour component into
																unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
			if (!success) {
				/* Error occured */
				fprintf(stderr, "Couldn't convert image");
				return false;
			}
			//glGenTextures(numTextures, &textureIds[i]); /* Texture name generation */
			glBindTexture(GL_TEXTURE_2D, textureIds[i]); /* Binding of texture name */
														 //redefine standard texture values
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
																			  interpolation for magnification filter */
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
																			  interpolation for minifying filter */
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
				ilGetData()); /* Texture specification */
		}
		else {
			/* Error occured */
			fprintf(stderr, "Couldn't load Image: %s\n", fileloc.data());
		}
	}
	ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data
										   we can release memory used by image. */

										   //Cleanup
	delete[] imageIds;
	imageIds = NULL;

	return true;
}

void Utils::AttachStream() {
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
	aiAttachLogStream(&stream);
}

void Utils::DetachStream() {
	aiDetachAllLogStreams();
}

void Utils::Log(std::string string) {
	std::cout << string << std::endl;
}

GLuint Utils::GenerateList(const aiScene* scene, const aiNode* node) {
	GLuint list = glGenLists(1);
	glNewList(list, GL_COMPILE);
	Utils::RecursiveRender(scene, node, 1.0);
	glEndList();
	return list;
}






