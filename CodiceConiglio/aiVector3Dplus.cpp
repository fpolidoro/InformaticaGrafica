#include "aiVector3Dplus.h"
#include "assimp/vector3.h"


aiVector3D operator - ( const aiVector3D& v1, const aiVector3D& v2)   {
    return aiVector3D( v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

aiVector3D operator ^ ( const aiVector3D& v1, const aiVector3D& v2)   {
    return aiVector3D( v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x);
}

aiVector3D operator * ( const float& p, const aiVector3D& v)   {
    return aiVector3D( p*v.x, p*v.y, p*v.z);
}

aiVector3D operator + ( const aiVector3D& v1, const aiVector3D& v2)   {
    return aiVector3D( v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

float dot(const aiVector3D& v1, const aiVector3D& v2) {
    return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

aiVector3D normal(const aiVector3D& v00, const aiVector3D& v1, const aiVector3D& v2){
	return (v1 - v00)^(v2-v00);
}
