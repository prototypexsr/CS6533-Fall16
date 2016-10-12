#pragma once
#include "matrix4.h"
#include "geometrymaker.h"

struct VertexPN {
	Cvec3f p, n;
	VertexPN() {}
	VertexPN(float x, float y, float z, float nx, float ny, float nz) : p(x, y, z), n(nx, ny, nz) {}
	VertexPN& operator = (const GenericVertex& v) {
		p = v.pos;
		n = v.normal;
		return *this;
	}
};

struct Transform {
	Quat rotation;
	Cvec3 scale;
	Cvec3 position;
	Transform() : scale(1.0f, 1.0f, 1.0f) {
	}
	Matrix4 createMatrix();
};
struct Geometry {
	GLuint vertexBO;
	GLuint indexBO;
	int numIndices;
	void Draw(GLuint positionAttribute, GLuint normalAttribute) {
		// bind buffer objects and draw
	}
};
struct Entity {
	Transform transform;
	Geometry geometry;
	void Draw(Matrix4 &eyeInverse, GLuint positionAttribute, GLuint normalAttribute,
		GLuint modelviewMatrixUniformLocation, GLuint normalMatrixUniformLocation) {
		// create modelview matrix
		// create normal matrix
		// set the model view and normal matrices to the uniforms locations
		geometry.Draw(positionAttribute, normalAttribute);
	}
};
