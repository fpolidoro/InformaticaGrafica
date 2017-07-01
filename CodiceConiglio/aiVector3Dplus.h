#ifndef AIVECTOR3DPLUS_H
#define AIVECTOR3DPLUS_H

#include "assimp/vector3.h"

aiVector3D operator - ( const aiVector3D& v1, const aiVector3D& v2);

aiVector3D operator ^ ( const aiVector3D& v1, const aiVector3D& v2);

aiVector3D operator * ( const float& p, const aiVector3D& v);

aiVector3D operator + ( const aiVector3D& v1, const aiVector3D& v2);

aiVector3D normal(const aiVector3D& v00, const aiVector3D& v1, const aiVector3D& v2);
float dot(const aiVector3D& v1, const aiVector3D& v2);

#endif
