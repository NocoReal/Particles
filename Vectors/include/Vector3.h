#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>

struct Vector3 {
	float x, y, z;

	Vector3(float x = 0, float y = 0, float z = 0);

	Vector3 operator+(const Vector3 v1) const;
	Vector3 operator-(const Vector3 v1) const;
	Vector3 operator*(const Vector3 v1) const;
	Vector3 operator/(const Vector3 v1) const;

	Vector3 operator*(float scalar) const;
	Vector3 operator/(float scalar) const;

	bool operator==(const Vector3& v1) const;  // Added equality operator
	bool operator!=(const Vector3& v1) const;  // Added inequality operator
	bool operator&&(const Vector3& v1) const;  // Logical AND operator
	bool operator||(const Vector3& v1) const;  // Logical OR operator

	float Magnitude() const;
	static float Distance(const Vector3 v1, const Vector3 v2);

	void Normalize(); //normalizes the vector
	Vector3 Normalized(); //returns that vector as a normalized vector
	
	void Invert(); //inverts the vector
	Vector3 Inverted(); //returns that vector as an inverted vector

	void RotateZ(float Theta);//rotates CCW on the z axis by Theta degrees
	Vector3 RotatedZ(float Theta);//returns the vector rotated CCW on the z axis by Theta degrees

	static Vector3 Up() { return Vector3(0, 1, 0); }
    static Vector3 Down() { return Vector3(0, -1, 0); }
    static Vector3 Left() { return Vector3(-1, 0, 0); }
    static Vector3 Right() { return Vector3(1, 0, 0); }
    static Vector3 Forward() { return Vector3(0, 0, 1); }
    static Vector3 Backward() { return Vector3(0, 0, -1); }

	// Convert a Vector3 to GLfloat array (size 3)
	void ToGLFloatArray(GLfloat* outArray) const {
		outArray[0] = x;
		outArray[1] = y;
		outArray[2] = z;
	}

	// Convert a vector of Vector3 to GLfloat array (flattened)
	static void ToGLFloatArray(const std::vector<Vector3>& vecs, GLfloat* outArray) {
		for (size_t i = 0; i < vecs.size(); ++i) {
			outArray[i * 3] = vecs[i].x;
			outArray[i * 3 + 1] = vecs[i].y;
			outArray[i * 3 + 2] = vecs[i].z;
		}
	}
};



float Dot(const Vector3 v1, const Vector3 v2);
float AngleBetween(const Vector3 v1, const Vector3 v2);

std::ostream& operator<<(std::ostream& os, const Vector3& vec);

#endif
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif