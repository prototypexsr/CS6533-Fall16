#pragma once
#include "matrix4.h"
#include "geometrymaker.h"

struct VertexPrime {
	Cvec3f p, n, b ,tg;
	Cvec2f t;
	VertexPrime() {}
	VertexPrime(float x, float y, float z, float nx, float ny, float nz) : p(x, y, z), n(nx, ny, nz) {}
	VertexPrime& operator = (const GenericVertex& v) {
		p = v.pos;
		n = v.normal;
		t = v.tex;
		b = v.binormal;
		tg = v.tangent;
		return *this;
	}
};

struct Transform {
	Quat rotationX, rotationY, rotationZ;
	Cvec3 scale;
	Cvec3 position;
	Matrix4 eyeMatrix, objectMatrix, modelviewMatrix, projectionMatrix, normalMatrix, rotationMatrix;
	Transform() : scale(1.0f, 1.0f, 1.0f) {
	}
	Matrix4 createMatrix() {
		
	}
};
struct Geometry {
	GLuint vertexBO;
	GLuint indexBO;
	int numIndices;
	void Draw(GLuint positionAttribute, GLuint normalAttribute) {
		// bind buffer objects and draw
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPrime), (void*)offsetof(VertexPrime, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPrime), (void*)offsetof(VertexPrime, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);

		/*glEnableVertexAttribArray(binormalAttribute);
		glVertexAttribPointer(binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBTG), (void*)offsetof(VertexPNTBTG, b));
		glEnableVertexAttribArray(binormalAttribute);

		glEnableVertexAttribArray(tangentAttribute);
		glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPNTBTG), (void*)offsetof(VertexPNTBTG, tg));
		glEnableVertexAttribArray(tangentAttribute);*/
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

	}
};
struct Entity {
	Transform transform;
	Geometry geometry;
	Entity* child;
	Entity* parent;
	float r, g, b;
	GLuint texture;

	//if (parent != nullptr) 
	void adoption(Entity* theChild) {
		theChild->parent = this;
		child = theChild;
	}
	void disown(Entity* theChild) {
		theChild->parent = nullptr;
		child = nullptr;
	}
	void matrixStacking(Entity* parent) {
		if (parent == nullptr) { return; }
		parent->transform.modelviewMatrix *= transform.modelviewMatrix;
		transform.modelviewMatrix = parent->transform.modelviewMatrix;

		matrixStacking(parent->parent);
	}
	void Draw(Matrix4 &eyeInverse, GLuint positionAttribute, GLuint normalAttribute,
		GLuint modelviewMatrixUniformLocation, GLuint normalMatrixUniformLocation, GLuint colorUniformLocation, float r, float g, float b) {
		// create modelview matrix
		glUniform3f(colorUniformLocation, r, g, b);
		transform.modelviewMatrix = eyeInverse;
		GLfloat glmatrix[16];
		transform.modelviewMatrix.writeToColumnMajorMatrix(glmatrix);
		glUniformMatrix4fv(modelviewMatrixUniformLocation, 1, false, glmatrix);// set the model view and normal matrices to the uniforms locations
		// create normal matrix
		Matrix4 normalMatrix;
		normalMatrix = transpose(inv(transform.modelviewMatrix));
		GLfloat glmatrixNormal[16];
		normalMatrix.writeToColumnMajorMatrix(glmatrixNormal);
		glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glmatrixNormal);// set the model view and normal matrices to the uniforms locations
		

		geometry.Draw(positionAttribute, normalAttribute);
		if (parent != nullptr) {
			//parent->transform.modelviewMatrix *= transform.modelviewMatrix;
			matrixStacking(parent);
			parent->Draw(eyeInverse, positionAttribute, normalAttribute, modelviewMatrixUniformLocation,
				normalMatrixUniformLocation, colorUniformLocation, parent->r, parent->g, parent->b);
		}
		
	}
	
};
