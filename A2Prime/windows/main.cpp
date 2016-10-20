#include "glsupport.h" //include this file first
#include "matrix4.h"
#include "quat.h"
#include "geometrymaker.h"
#include "VertexPN.h"
#include <GL\freeglut.h>


GLuint program; 
GLuint vertPositionVBO, vertTexCoordVBO, colorVBO, normalVBO;
GLuint positionAttribute, colorAttribute, texCoordAttribute, normalAttribute;
GLuint texture;
GLuint positionUniform, timeUniform;
GLuint modelviewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint colorUniformLocation, normalUniformLocation;
 
float textureOffset = 0.0;
Entity ball;
int ibLen2, vbLen2;
Entity cube;
int ibLen, vbLen;
Entity bigCube;
int ibLen3, vbLen3;

 
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUniform1f(timeUniform, textureOffset);
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(timeUniform, (float)timeSinceStart / 1000.0f);
	
	
	
	

	
	

	Matrix4 objectMatrix;
	//objectMatrix = objectMatrix.makeXRotation(0.01 * timeSinceStart);
	//objectMatrix = objectMatrix.makeYRotation(-0.02 * timeSinceStart );
	Quat q1 = Quat::makeXRotation( 0.001* timeSinceStart *70.0f);
	Quat q2 = Quat::makeZRotation(0.001 * timeSinceStart * 20.0f);
	Quat q3 = q1;
	Quat q4 = Quat::makeZRotation(-0.001 * timeSinceStart * 20.0f);
	Quat combined = q1 * q2;
	Quat combined2 = q3 * q4;
	Matrix4 rotationMatrix = quatToMatrix(combined);
	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.00, 0.0, 3.00)); //z-value will decide how "far" away the object is

	Matrix4 modelViewMatrix = inv(eyeMatrix) * rotationMatrix; //keep eye matrix inverted at all times
	
	Matrix4 modelViewMatrix2 = inv(eyeMatrix) * quatToMatrix(combined2);


	//Matrix4 modelViewMatrix; 
	//modelViewMatrix = normalMatrix;
	//GLfloat glmatrix[16];
	//modelViewMatrix.writeToColumnMajorMatrix(glmatrix);
	////normalMatrix.writeToColumnMajorMatrix(glmatrix);
	//glUniformMatrix4fv(modelviewMatrixUniformLocation, 1, false, glmatrix);
	//glUniformMatrix4fv(normalUniformLocation, 1, false, glmatrix);
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(90.0, 1.0, -0.1, -100.0); //vertical field of view is important (1st parameter)! higher vfov will allow for better perephial vision
	//Vertical Field of View = 2 * arctan( tan(hfox/2) * Aspect Ratio)
	GLfloat glmatrixProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

	/*Matrix4 normalMatrix;
	normalMatrix = transpose(inv(modelViewMatrix));
	GLfloat glmatrixNormal[16];
	normalMatrix.writeToColumnMajorMatrix(glmatrixNormal);
	glUniformMatrix4fv(normalUniformLocation, 1, false, glmatrixNormal);*/

	/*glUniform3f(colorUniformLocation, 0.0, 0.45, 1.0);*/
	cube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 0.0, 0.45, 1.0);
	ball.Draw(inv(modelViewMatrix), positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.45, 0.0);
	bigCube.Draw(modelViewMatrix2, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 0.1, 0.2, 0.4);
	

	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);

	

	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(normalAttribute);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	

    glutSwapBuffers();
}


void init() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glReadBuffer(GL_BACK);

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


	/*Entity cube;
	int ibLen, vbLen;
	getCubeVbIbLen(vbLen, ibLen);
	std::vector<VertexPN> vtx(vbLen);
	std::vector<unsigned short> idx(ibLen);
	makeCube(2, vtx.begin(), idx.begin());
	cube.geometry.indexBO = ibLen;
	cube.geometry.vertexBO = vbLen;
	cube.geometry.numIndices = ibLen;*/
	//cube.geometry.Draw(positionAttribute, normalAttribute);
	cube.child = &bigCube;
	bigCube.child = &ball;
	//getCubeVbIbLen(vbLen2, ibLen2);
	getCubeVbIbLen(vbLen, ibLen);
	cube.geometry.indexBO = ibLen;
	cube.geometry.vertexBO = vbLen;
	cube.geometry.numIndices = ibLen;

	std::vector<VertexPN> vtx(vbLen);
	std::vector<unsigned short> idx(ibLen);

	makeCube(1, vtx.begin(), idx.begin());


	getSphereVbIbLen(20, 20, vbLen2, ibLen2);
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




	/*glGenBuffers(1, &vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
*/
	/*glGenBuffers(1, &vertPositionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);

	GLfloat cubeVerts[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);*/
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


	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);

	GLfloat cubeColors[] = {
		0.583f, 0.771f, 0.014f, 1.0f,
		0.609f, 0.115f, 0.436f, 1.0f,
		0.327f, 0.483f, 0.844f, 1.0f,
		0.822f, 0.569f, 0.201f, 1.0f,
		0.435f, 0.602f, 0.223f, 1.0f,
		0.310f, 0.747f, 0.185f, 1.0f,
		0.597f, 0.770f, 0.761f, 1.0f,
		0.559f, 0.436f, 0.730f, 1.0f,
		0.359f, 0.583f, 0.152f, 1.0f,
		0.483f, 0.596f, 0.789f, 1.0f,
		0.559f, 0.861f, 0.639f, 1.0f,
		0.195f, 0.548f, 0.859f, 1.0f,
		0.014f, 0.184f, 0.576f, 1.0f,
		0.771f, 0.328f, 0.970f, 1.0f,
		0.406f, 0.615f, 0.116f, 1.0f,
		0.676f, 0.977f, 0.133f, 1.0f,
		0.971f, 0.572f, 0.833f, 1.0f,
		0.140f, 0.616f, 0.489f, 1.0f,
		0.997f, 0.513f, 0.064f, 1.0f,
		0.945f, 0.719f, 0.592f, 1.0f,
		0.543f, 0.021f, 0.978f, 1.0f,
		0.279f, 0.317f, 0.505f, 1.0f,
		0.167f, 0.620f, 0.077f, 1.0f,
		0.347f, 0.857f, 0.137f, 1.0f,
		0.055f, 0.953f, 0.042f, 1.0f,
		0.714f, 0.505f, 0.345f, 1.0f,
		0.783f, 0.290f, 0.734f, 1.0f,
		0.722f, 0.645f, 0.174f, 1.0f,
		0.302f, 0.455f, 0.848f, 1.0f,
		0.225f, 0.587f, 0.040f, 1.0f,
		0.517f, 0.713f, 0.338f, 1.0f,
		0.053f, 0.959f, 0.120f, 1.0f,
		0.393f, 0.621f, 0.362f, 1.0f,
		0.673f, 0.211f, 0.457f, 1.0f,
		0.820f, 0.883f, 0.371f, 1.0f,
		0.982f, 0.099f, 0.879f, 1.0f
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);


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