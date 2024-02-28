#ifndef MAT3_H
#define MAT3_H

#include "Vec3.h"

class CMat3
{
public:
    CMat3();
	CMat3(float,float,float,float,float,float,float,float,float);
	CVec3 operator*(CVec3& vector);
	void TransformationMatrix(CVec3& v1,CVec3& v2,CVec3& v3);
	void RotationMatrix(float a,CVec3& r);
	void print();

	float m11; float m12; float m13;
	float m21; float m22; float m23;
	float m31; float m32; float m33;
};


#endif
