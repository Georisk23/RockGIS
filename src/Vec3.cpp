#ifndef VEC3_CPP
#define VEC3_CPP

#include <iomanip> //setprecision
#include <iostream>
#include <math.h>

#include "Vec3.h"



CVec3::CVec3(){}

CVec3::CVec3(float xi, float yi, float zi){
	x=xi;y=yi;z=zi;
}

bool CVec3::operator==(CVec3 rhs) {
    return(x == rhs.x && y == rhs.y && z == rhs.z);
}

CVec3 CVec3::operator+(CVec3 rhs) {
    return CVec3( x + rhs.x, 
                 y + rhs.y, 
                 z + rhs.z);
}

CVec3 CVec3::operator-(CVec3 rhs) {
    return CVec3( x - rhs.x, 
                 y - rhs.y, 
                 z - rhs.z);
}

CVec3 CVec3::operator*(float scalar) {
    return CVec3(x * scalar, 
                y * scalar,
                z * scalar);
}

CVec3 CVec3::operator/(float scalar) {
    return CVec3(x / scalar, 
                y / scalar,
                z / scalar);
}

float CVec3::dot(CVec3 rhs) {
    return (x * rhs.x + 
            y * rhs.y + 
            z * rhs.z);
}

CVec3 CVec3::cross(CVec3 rhs) {
    return CVec3( y * rhs.z - z * rhs.y,
                 z * rhs.x - x * rhs.z,
                 x * rhs.y - y * rhs.x);
}

float CVec3::length() {
    return float(sqrt( x*x + y*y + z*z ));
}
void CVec3::print(){
	std::cout << std::setprecision(8) << x << " , " << y << " , " << z << "\n";
}

#endif
