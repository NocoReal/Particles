#include "Vector3.h"
#include <cmath>
#include <vector>
#include <GLFW/glfw3.h>

using namespace std;

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z){}

Vector3 Vector3::operator+(const Vector3 v1) const {
	return Vector3(x + v1.x, y + v1.y, z + v1.z);
}
Vector3 Vector3::operator-(const Vector3 v1) const {
	return Vector3(x - v1.x, y - v1.y, z - v1.z);
}
Vector3 Vector3::operator*(const Vector3 v1) const {
	return Vector3(x * v1.x, y * v1.y, z * v1.z);
}
Vector3 Vector3::operator/(const Vector3 v1) const {
	return Vector3(x / v1.x, y / v1.y, z / v1.z);
}

Vector3 Vector3::operator*(float scalar) const {
	return Vector3(x * scalar, y * scalar, z * scalar);
}
Vector3 Vector3::operator/(float scalar) const {
	return Vector3(x/scalar, y/scalar, z / scalar);
}

Vector3 operator*(float scalar, const Vector3 v1){
	return Vector3(scalar * v1.x, scalar * v1.y, scalar * v1.z);
}
Vector3 operator/(float scalar, const Vector3 v1){
	return Vector3(scalar / v1.x,  scalar /v1.y, scalar / v1.z) ;
}

float Vector3::Magnitude() const {
	return abs(sqrt(x * x + y * y + z * z));
}

float Vector3::Distance(const Vector3 v1,const Vector3 v2){
	Vector3 tV = v1 - v2;
	return tV.Magnitude();
}


void Vector3::Normalize(){
	float magnitude = Magnitude();
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
}
Vector3 Vector3::Normalized(){
	Vector3 tV1 = Vector3(x, y, z);
	tV1.Normalize();
	return tV1;
}

void Vector3::Invert(){
	x = -x;
	y = -y;
	z = -z;
}
Vector3 Vector3::Inverted(){
	Vector3 tV1 = Vector3(x, y, z);
	tV1.Invert();
	return tV1;
}

void Vector3::RotateZ(float Theta) {
	float radTheta = Theta * (M_PI / 180);
	float oldX = x; // Store original x
	float oldY = y; // Store original y
	x = oldX * cos(radTheta) - oldY * sin(radTheta);
	y = oldX * sin(radTheta) + oldY * cos(radTheta);
	if (fabs(x) < 1e-5f) x = 0;
	if (fabs(y) < 1e-5f) y = 0; 
}
Vector3 Vector3::RotatedZ(float Theta) {
	Vector3 tV1 = Vector3(x,y,z);
	tV1.RotateZ(Theta);
	return tV1;
}

float Dot(const Vector3 v1, const Vector3 v2){
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
float AngleBetween(const Vector3 v1, const Vector3 v2){
		return acos(Dot(v1, v2) / (v1.Magnitude() * v2.Magnitude())) * (180 / M_PI);
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
	os << "(" << vec.x << ", " << vec.y <<", "<< vec.z << ")";
	return os;
}

bool Vector3::operator==(const Vector3& v1) const {
	return x == v1.x && y == v1.y && z == v1.z;
}

bool Vector3::operator!=(const Vector3& v1) const {
	return !(*this == v1);
}

bool Vector3::operator&&(const Vector3& v1) const {
	return (x != 0 && y != 0 && z != 0) && (v1.x != 0 && v1.y != 0 && v1.z != 0);
}

bool Vector3::operator||(const Vector3& v1) const {
	return (x != 0 || y != 0 || z != 0) || (v1.x != 0 || v1.y != 0 || v1.z != 0);
}