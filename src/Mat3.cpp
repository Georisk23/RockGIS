#ifndef MAT3_CPP
#define MAT3_CPP

#include <iostream>
#include <math.h>


#include "Vec3.h"
#include "Mat3.h"

CMat3::CMat3(){}

CMat3::CMat3(float m11i, float m12i, float m13i,float m21i, float m22i, float m23i,float m31i, float m32i, float m33i){
	m11=m11i;m12=m12i;m13=m13i;m21=m21i;m22=m22i;m23=m23i;m31=m31i;m32=m32i;m33=m33i;
}

CVec3 CMat3::operator*(CVec3& v) {
    return CVec3(
				m11*v.x+m12*v.y+m13*v.z, 
                m21*v.x+m22*v.y+m23*v.z,
                m31*v.x+m32*v.y+m33*v.z
                );
}

void CMat3::TransformationMatrix(CVec3& v1,CVec3& v2,CVec3& v3) {
    m11=v1.x;m12=v2.x;m13=v3.x;
    m21=v1.y;m22=v2.y;m23=v3.y;
    m31=v1.z;m32=v2.z;m33=v3.z;
}

void CMat3::RotationMatrix(float a,CVec3& r) {
	
	m11=(r.x*r.x*(1.0-cos(a)))+cos(a);
	m12=(r.x*r.y*(1.0-cos(a)))-r.z*sin(a);
	m13=(r.x*r.z*(1.0-cos(a)))+r.y*sin(a);
	
    m21=(r.x*r.y*(1.0-cos(a)))+r.z*sin(a);
    m22=(r.y*r.y*(1.0-cos(a)))+cos(a);
    m23=(r.y*r.z*(1.0-cos(a)))-r.x*sin(a);
    
    m31=(r.x*r.z*(1.0-cos(a)))-r.y*sin(a);
    m32=(r.y*r.z*(1.0-cos(a)))+r.x*sin(a);
    m33=(r.z*r.z*(1.0-cos(a)))+cos(a);		
}

void CMat3::print(){
	std::cout << "[\n";
	std::cout << m11 << " , " << m12 << " , " << m13 << "\n";
	std::cout << m21 << " , " << m22 << " , " << m23 << "\n";
	std::cout << m31 << " , " << m32 << " , " << m33 << "\n";
	std::cout << "]\n";
}



#endif
