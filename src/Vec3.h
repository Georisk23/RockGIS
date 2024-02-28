#ifndef VEC3_H
#define VEC3_H

class CVec3
{
public:
	CVec3();
	CVec3(float, float, float);
	bool operator==(CVec3 rhs);
	CVec3 operator+(CVec3 rhs);
	CVec3 operator-(CVec3 rhs);
	CVec3 operator*(float scalar);
	CVec3 operator/(float scalar);
	CVec3 cross(CVec3 rhs);
	float dot(CVec3 rhs);
	float length();
	void print();
	
	float x;
	float y;
	float z;
};

#endif
