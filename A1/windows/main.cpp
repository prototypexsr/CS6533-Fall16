#include "glsupport.h" //include this file first
#include <GL\freeglut.h>


GLuint program; 
GLuint vertPositionVBO, vertTexCoordVBO;
GLuint positionAttribute, colorAttribute, texCoordAttribute;
GLuint texture;
GLuint positionUniform, timeUniform;
 
float textureOffset = 0.0;
 
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	//glUseProgram(program); 
	glUniform1f(timeUniform, textureOffset);
	/*int timeSinceStart = glutGet(GLUT_ELAPSED_TIME);     
	glUniform1f(timeUniform, (float)timeSinceStart / 1000.0f);*/

	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);   
	glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);    
	glEnableVertexAttribArray(positionAttribute); 

	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);     
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);    
	glEnableVertexAttribArray(texCoordAttribute);     
	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform2f(positionUniform, -0.5, 0.0);
	glDrawArrays(GL_TRIANGLES, 0, 6);  


	glUniform2f(positionUniform, 0.5, 0.0); //without this, drawing a second image will place it on top (or behind) of the previous image
	glDrawArrays(GL_TRIANGLES, 0, 6);


	glEnable(GL_BLEND);    
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0, 0.0, 0.0, 1.0);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(texCoordAttribute);

    glutSwapBuffers();
}
void mouse(int button, int state, int x, int y) { 
	float newPositionX = (float)x / 250.0f - 1.0f;   
	float newPositionY = (1.0 - (float)y / 250.0);  
	glUniform2f(positionUniform, newPositionX, newPositionY); 
} 
void mouseMove(int x, int y) { 
	float newPositionX = (float)x / 250.0f - 1.0f;    
	float newPositionY = (1.0 - (float)y / 250.0);    
	glUniform2f(positionUniform, newPositionX, newPositionY); 
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) { case 'a':   
		textureOffset -= 0.12;       
		break;        
	case 'd':       
		textureOffset += 0.12;   
		break; 
	} 
}

void init() {
	program = glCreateProgram();    
	readAndCompileShader(program, "vertex_textured.glsl", "fragment_textured.glsl");   
	texture = loadGLTexture("paragonapp.png");

	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	texCoordAttribute = glGetAttribLocation(program, "texCoord");
	//colorAttribute = glGetAttribLocation(program, "color");
	//positionUniform = glGetUniformLocation(program, "position");
	timeUniform = glGetUniformLocation(program, "time");
	
	glGenBuffers(1, &vertPositionVBO);    
	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);     
	GLfloat sqVerts[12] = { -0.5f, -0.5f,         
		0.5f, 0.5f,         
		0.5f, -0.5f,           
		-0.5f, -0.5f,        
		-0.5f, 0.5f,       
		0.5f, 0.5f };   
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqVerts, GL_STATIC_DRAW);

	glGenBuffers(1, &vertTexCoordVBO);     
	glBindBuffer(GL_ARRAY_BUFFER, vertTexCoordVBO);     
	GLfloat sqTexCoords[12] = { 0.0f, 1.0f,        
		1.0f, 0.0f,       
		1.0f, 1.0f,                 
		0.0f, 1.0f,        
		0.0f, 0.0f,         
		1.0f, 0.0f, };    
	
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), sqTexCoords, GL_STATIC_DRAW);

} 


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void idle(void) {
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("CS-6533");

    glewInit();
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);     
	glutMotionFunc(mouseMove);
    
    init();
    glutMainLoop();
    return 0;
}