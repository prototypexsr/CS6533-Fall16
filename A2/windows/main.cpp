#include "glsupport.h" //include this file first
#include "matrix4.h"
#include <GL\freeglut.h>


GLuint program; 
GLuint vertPositionVBO, vertTexCoordVBO, colorVBO;
GLuint positionAttribute, colorAttribute, texCoordAttribute;
GLuint texture;
GLuint positionUniform, timeUniform;
GLuint modelviewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
 
float textureOffset = 0.0;
 
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUniform1f(timeUniform, textureOffset);
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(timeUniform, (float)timeSinceStart / 1000.0f);

	Matrix4 objectMatrix;
	//objectMatrix = objectMatrix.makeXRotation(0.01 * timeSinceStart);
	objectMatrix = objectMatrix.makeYRotation(0.01 * timeSinceStart );
	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.00, 0.0, 2.90)); //z-value will decide how "far" away the object is
	Matrix4 modelViewMatrix = inv(eyeMatrix) * objectMatrix; //keep eye matrix inverted at all times
	//Matrix4 modelViewMatrix;
	GLfloat glmatrix[16];
	modelViewMatrix.writeToColumnMajorMatrix(glmatrix);
	glUniformMatrix4fv(modelviewMatrixUniformLocation, 1, false, glmatrix);
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeProjection(90.0, 1.0, -0.1, -100.0); //vertical field of view is important (1st parameter)! higher vfov will allow for better perephial vision
	GLfloat glmatrixProjection[16];
	projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);


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

	
	modelviewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");

	glGenBuffers(1, &vertPositionVBO);
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

	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), cubeVerts, GL_STATIC_DRAW);

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

	glBufferData(GL_ARRAY_BUFFER, 144 * sizeof(GLfloat), cubeColors, GL_STATIC_DRAW);

	

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