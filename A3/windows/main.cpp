/*Stefan Cherubin HW2
Primitives used: 2 cubes, 1 sphere
sphere is inside a cube, which is inside a larger cube
*/
#define TINYOBJLOADER_IMPLEMENTATION
#include "glsupport.h" //include this file first
#include "matrix4.h"
#include "quat.h"
#include "geometrymaker.h"
#include "VertexPN.h"
#include "tiny_obj_loader.h"
#include <GL\freeglut.h>


GLuint program; 
GLuint vertPositionVBO, vertTexCoordVBO, colorVBO, normalVBO;
GLuint positionAttribute, colorAttribute, texCoordAttribute, normalAttribute;
GLuint texture;
GLuint positionUniform, timeUniform, lightDirectionUniform;
GLuint modelviewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint colorUniformLocation, normalUniformLocation, lightDirectionUniformLocation0, lightDirectionUniformLocation1;
 
float textureOffset = 0.0;
Entity ball;
int ibLen2, vbLen2;
Entity cube;
int ibLen, vbLen;
Entity bigCube;
int ibLen3, vbLen3;
Entity object;



 
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUniform1f(timeUniform, textureOffset);
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(timeUniform, (float)timeSinceStart / 1000.0f);
	
	
	
	

	
	

	Matrix4 objectMatrix;
	//objectMatrix = objectMatrix.makeXRotation(0.01 * timeSinceStart);
	//objectMatrix = objectMatrix.makeYRotation(-0.02 * timeSinceStart );
	//Quat q1 = Quat::makeXRotation( 0.001* timeSinceStart *70.0f);
	Quat q2 = Quat::makeYRotation(0.001 * timeSinceStart * 20.0f);
	//Quat q3 = q1;
	Quat q4 = Quat::makeZRotation(-0.001 * timeSinceStart * 20.0f);
	Quat combined = q2;
	//Quat combined2 = q3 * q4;
	Matrix4 rotationMatrix = quatToMatrix(combined);
	Matrix4 eyeMatrix;
	//eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.00, 0.05, 0.25)); //z-value will decide how "far" away the object is
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.00, 1.05, 3.25)); //z-value will decide how "far" away the object is

	Matrix4 modelViewMatrix = inv(eyeMatrix) * rotationMatrix; //keep eye matrix inverted at all times
	
	//Matrix4 modelViewMatrix2 = inv(eyeMatrix) * quatToMatrix(combined2);


	//Should move matrix stuff inside Entity class
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(90.0, 1.0, -0.1, -100.0); //vertical field of view is important (1st parameter)! higher vfov will allow for better perephial vision
	//Vertical Field of View = 2 * arctan( tan(hfox/2) * Aspect Ratio)
	GLfloat glmatrixProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);
	ball.r = 1.0;
	cube.g = 1.0;
	bigCube.b = 1.0;

	//For now, all three entites will share everything, except for color attribute
	//cube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 0.0, 0.0, 0.0);
	//bigCube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.0, 1.0);
	//ball.Draw(modelViewMatrix , positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.45, 0.0);
	//object.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.1, 0.1);
	ball.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, ball.r, ball.g, ball.b);

	

	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);

	
	//glDrawArrays replaced by glDrawElement in Geometry class
	//glDrawArrays(GL_TRIANGLES, 0, meshVertices.size());
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(normalAttribute);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	

    glutSwapBuffers();
}

void loadObjFile(const std::string &fileName, std::vector<VertexPN> &outVertices, std::vector<unsigned short> &outIndices)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
	if (ret) {
		for (int i = 0; i < attrib.vertices.size(); i += 3)
		{
			VertexPN v;
			v.p[0] = attrib.vertices[i];
			v.p[1] = attrib.vertices[i + 1];
			v.p[2] = attrib.vertices[i + 2];
			v.n[0] = attrib.normals[i];
			v.n[1] = attrib.normals[i + 1];
			v.n[2] = attrib.normals[i + 2];
			outVertices.push_back(v);
		}
		for (int i = 0; i < shapes.size(); i++)
		{
			for (int j = 0; j < shapes[i].mesh.indices.size(); j++)
			{
				outIndices.push_back(shapes[i].mesh.indices[j].vertex_index);
			}
		}
	}
	else {
		std::cout << err << std::endl;
		assert(false);
	}
}

void init() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //GL_BACK will enable that nice lighting effect, but you may be able to see through your object
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glReadBuffer(GL_BACK);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	program = glCreateProgram();
	readAndCompileShader(program, "vertex_textured.glsl", "fragment_textured.glsl");
	glUseProgram(program);

	positionAttribute = glGetAttribLocation(program, "position");
	colorAttribute = glGetAttribLocation(program, "color");
	timeUniform = glGetUniformLocation(program, "time");
	normalAttribute = glGetAttribLocation(program, "normal");

	normalUniformLocation = glGetUniformLocation(program, "normalMatrix");
	colorUniformLocation = glGetUniformLocation(program, "uColor");
	modelviewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");

	lightDirectionUniformLocation0 = glGetUniformLocation(program, "lights[0].lightDirection");
	lightDirectionUniformLocation1 = glGetUniformLocation(program, "lights[1].lightDirection");
	//lightDirectionUniformLocation1
	//lightDirectionUniform = glGetUniformLocation(program, "lightDirection");


	
	//cube.adoption(&bigCube);
	//bigCube.adoption(&ball);
	bigCube.adoption(&cube);
	cube.adoption(&ball);
	
	getCubeVbIbLen(vbLen, ibLen);
	cube.geometry.indexBO = ibLen;
	cube.geometry.vertexBO = vbLen;
	cube.geometry.numIndices = ibLen;

	std::vector<VertexPN> vtx(vbLen);
	std::vector<unsigned short> idx(ibLen);

	makeCube(1, vtx.begin(), idx.begin());


	getSphereVbIbLen(20, 20, vbLen2, ibLen2); //more slices & stacks make it more smooth and circle like
	ball.geometry.indexBO = ibLen2;
	ball.geometry.vertexBO = vbLen2;
	ball.geometry.numIndices = ibLen2;

	std::vector<VertexPN> vtx2(vbLen2);
	std::vector<unsigned short> idx2(ibLen2);

	makeSphere(0.5, 20, 20, vtx2.begin(), idx2.begin());

	getCubeVbIbLen(vbLen3, ibLen3);
	bigCube.geometry.indexBO = ibLen3;
	bigCube.geometry.vertexBO = vbLen3;
	bigCube.geometry.numIndices = ibLen3;

	std::vector<VertexPN> vtx3(vbLen3);
	std::vector<unsigned short> idx3(ibLen3);

	makeCube(2, vtx3.begin(), idx3.begin());



	glGenBuffers(1, &ball.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, ball.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx2.size(), vtx2.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &ball.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ball.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx2.size(), idx2.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &cube.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, cube.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &cube.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &bigCube.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, bigCube.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx3.size(), vtx3.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &bigCube.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bigCube.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx3.size(), idx3.data(), GL_STATIC_DRAW);




	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	GLfloat normalVerts[] = {
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		0.0f, 0.0f,-1.0f,
		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,
		-1.0f, 0.0f,0.0f,
		0.0f,-1.0f, 0.0f,
		0.0f,-1.0f, 0.0f,
		0.0f,-1.0f,0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f,0.0f, 1.0f,
		0.0f,0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f,0.0f,
		1.0f, 0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f,0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 1.0f,0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f,0.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(normalVerts), normalVerts, GL_STATIC_DRAW);

	
	std::vector<VertexPN> meshVertices;
	std::vector<unsigned short> meshIndices;
	loadObjFile("lucy.obj", meshVertices, meshIndices);
	object.geometry.numIndices = meshIndices.size();
	object.geometry.vertexBO = meshVertices.size();
	object.geometry.indexBO = meshIndices.size();
	glGenBuffers(1, &object.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, object.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * meshVertices.size(), meshVertices.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &object.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * meshIndices.size(), meshIndices.data(), GL_STATIC_DRAW);



	//object.geometry.numIndices = meshIndices.size();
	//object.geometry.vertexBO = meshVertices.size();
	

	
} 




void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CS-6533");

    glewInit();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

	//glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);     
	//glutMotionFunc(mouseMove);
    
    init();
    glutMainLoop();
    return 0;
}