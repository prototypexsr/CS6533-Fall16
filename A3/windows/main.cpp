/*Stefan Cherubin HW2
Primitives used: 2 cubes, 1 sphere
sphere is inside a cube, which is inside a larger cube
*/
#define TINYOBJLOADER_IMPLEMENTATION
#include "glsupport.h" //include this file first
#include "matrix4.h"
#include "quat.h"
#include "geometrymaker.h"
#include "VertexPrime.h"
#include "tiny_obj_loader.h"
#include "stb_image.h"
#include <GL\freeglut.h>


GLuint program; 
GLuint vertPositionVBO, vertTexCoordVBO, colorVBO, normalVBO;
GLuint positionAttribute, colorAttribute, texCoordAttribute, normalAttribute, binormalAttribute, tangentAttribute;
GLuint texture;
GLuint positionUniform, timeUniform, lightDirectionUniform;
GLuint modelviewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint colorUniformLocation, normalUniformLocation, lightDirectionUniformLocation0, lightDirectionUniformLocation1, normalTexUniformLocation;
GLuint diffuseTexture, specularTexture, diffuseTexture2, normalTexture;
GLuint diffuseTextureUniformLocation, diffuseTextureUniformLocation1;
GLuint specularUniformLocation;
GLuint cubeMap;
 
float textureOffset = 0.0;
Entity ball;
int ibLen2, vbLen2;
Entity cube;
int ibLen, vbLen;
Entity bigCube;
int ibLen3, vbLen3;
Entity object, object2;
Entity areaFloor;
int ibLen4, vbLen4;

//std::vector<unsigned short> cubeVertices;



 
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
	//eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.00, 1.05, 3.25)); //z-value will decide how "far" away the object is
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.00, 6.05, 10.25)); //z-value will decide how "far" away the object is

	Matrix4 modelViewMatrix = inv(eyeMatrix) * rotationMatrix; //keep eye matrix inverted at all times
	
	//Matrix4 modelViewMatrix = inv(eyeMatrix) * rotationMatrix;
	Matrix4 modelViewMatrix2 = inv(eyeMatrix) * quatToMatrix(combined);


	//Should move matrix stuff inside Entity class
	//Matrix4 projectionMatrix;
	//projectionMatrix = projectionMatrix.makeProjection(90.0, 1.0, -0.1, -100.0); //vertical field of view is important (1st parameter)! higher vfov will allow for better perephial vision
	////Vertical Field of View = 2 * arctan( tan(hfox/2) * Aspect Ratio)
	//GLfloat glmatrixProjection[16];
	//projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
	//glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);
	//object.transform.rotateYwithTime(timeUniform);
	object.transform.setEyeTranslation(8.0, 6.05, 10.25);
	object.transform.createProjectionMatrix(projectionMatrixUniformLocation);
	object.transform.setModelviewMatrix();
	object.transform.rotateYwithTime(timeUniform);

	object2.transform.setEyeTranslation(-8.0, 6.05, 10.25);
	object2.transform.createProjectionMatrix(projectionMatrixUniformLocation);
	object2.transform.setModelviewMatrix();
	object2.transform.rotateYwithTime(timeUniform);
	
	/*areaFloor.transform.setEyeTranslation(0.0, 7.05, 10.25);
	areaFloor.transform.createProjectionMatrix(projectionMatrixUniformLocation);
	areaFloor.transform.setModelviewMatrix();
	areaFloor.transform.rotateYwithTime(timeUniform);*/
	//areaFloor.transform.createProjectionMatrix(projectionMatrixUniformLocation);
	
	ball.r = 1.0;
	cube.g = 1.0;
	bigCube.b = 1.0;

	//For now, all three entites will share everything, except for color attribute
	//cube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 0.0, 0.0, 0.0);
	//bigCube.Draw(modelViewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.0, 1.0);
	//ball.Draw(modelViewMatrix , positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.45, 0.0);
	object.Draw(object.transform.modelviewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.1, 0.1);
	//areaFloor.Draw(areaFloor.transform.modelviewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, ball.r, ball.g, ball.b);
	object2.Draw(object2.transform.modelviewMatrix, positionAttribute, normalAttribute, modelviewMatrixUniformLocation, normalUniformLocation, colorUniformLocation, 1.0, 0.1, 0.1);

	

	glBindBuffer(GL_ARRAY_BUFFER, vertPositionVBO);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(normalAttribute);

	glEnableVertexAttribArray(binormalAttribute);
	glVertexAttribPointer(binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPrime), (void*)offsetof(VertexPrime, b));
	glDisableVertexAttribArray(binormalAttribute);

	glEnableVertexAttribArray(tangentAttribute);
	glVertexAttribPointer(tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPrime), (void*)offsetof(VertexPrime, tg));
	glDisableVertexAttribArray(tangentAttribute);
	//glDrawArrays replaced by glDrawElement in Geometry class
	//glDrawArrays(GL_TRIANGLES, 0, meshVertices.size());
	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(normalAttribute);

	

	//glUniform1i(diffuseTextureUniformLocation, 2);
	//glActiveTexture(GL_TEXTURE2);
	//glBindTexture(GL_TEXTURE_2D, areaFloor.texture);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);

	glClearColor(0.1, 0.1, 0.1, 1.0);

	

    glutSwapBuffers();
}
void calculateFaceTangent(const Cvec3f &v1, const Cvec3f &v2, const Cvec3f &v3, const Cvec2f &texCoord1, const Cvec2f &texCoord2,
	const Cvec2f &texCoord3, Cvec3f &tangent, Cvec3f &binormal) {
	Cvec3f side0 = v1 - v2;
	Cvec3f side1 = v3 - v1;
	Cvec3f normal = cross(side1, side0);
	normalize(normal);
	float deltaV0 = texCoord1[1] - texCoord2[1];
	float deltaV1 = texCoord3[1] - texCoord1[1];
	tangent = side0 * deltaV1 - side1 * deltaV0;
	normalize(tangent);
	float deltaU0 = texCoord1[0] - texCoord2[0];
	float deltaU1 = texCoord3[0] - texCoord1[0];
	binormal = side0 * deltaU1 - side1 * deltaU0;
	normalize(binormal);
	Cvec3f tangentCross = cross(tangent, binormal);
	if (dot(tangentCross, normal) < 0.0f) {
		tangent = tangent * -1;
	}
}

void loadObjFile(const std::string &fileName, std::vector<VertexPrime> &outVertices, std::vector<unsigned short> &outIndices) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, fileName.c_str(), NULL, true);
	if (ret) {
		for (int i = 0; i < shapes.size(); i++) {
			for (int j = 0; j < shapes[i].mesh.indices.size(); j++) {
				unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
				unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
				unsigned int texOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
				VertexPrime v;
				v.p[0] = attrib.vertices[vertexOffset];
				v.p[1] = attrib.vertices[vertexOffset + 1];
				v.p[2] = attrib.vertices[vertexOffset + 2];
				v.n[0] = attrib.normals[normalOffset];
				v.n[1] = attrib.normals[normalOffset + 1];
				v.n[2] = attrib.normals[normalOffset + 2];
				v.t[0] = attrib.texcoords[texOffset];
				v.t[1] = 1.0 - attrib.texcoords[texOffset + 1];
				outVertices.push_back(v);
				outIndices.push_back(outVertices.size() - 1);


			}

		}
		for (int i = 0; i < outVertices.size(); i += 3) {
			Cvec3f tangent;
			Cvec3f binormal;
			calculateFaceTangent(outVertices[i].p, outVertices[i + 1].p, outVertices[i + 2].p,
				outVertices[i].t, outVertices[i + 1].t, outVertices[i + 2].t, tangent, binormal);
			outVertices[i].tg = tangent;
			outVertices[i + 1].tg = tangent;
			outVertices[i + 2].tg = tangent;
			outVertices[i].b = binormal;
			outVertices[i + 1].b = binormal;
			outVertices[i + 2].b = binormal;
		}
		/*std::vector<std::string> cubemapFiles;
		cubemapFiles.push_back("alps_ft.tga");
		cubemapFiles.push_back("alps_bk.tga");
		cubemapFiles.push_back("alps_up.tga");
		cubemapFiles.push_back("alps_dn.tga");
		cubemapFiles.push_back("alps_rt.tga");
		cubemapFiles.push_back("alps_lf.tga");
		cubeMap = loadGLCubemap(cubemapFiles);*/
		
	}
	else {
		std::cout << err << std::endl;
		assert(false);
	}
}
GLuint loadGLCubemap(std::vector<std::string> faces) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++) {
		int w, h, comp;
		unsigned char* image = stbi_load(faces[i].c_str(), &w, &h, &comp, STBI_rgb_alpha);
		if (image) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image
			);
			stbi_image_free(image);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;
}
void init() {
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); //GL_BACK will enable that nice lighting effect, but you may be able to see through your object
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glReadBuffer(GL_BACK);
	glClearDepth(0.0f);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);

	program = glCreateProgram();
	readAndCompileShader(program, "vertex_textured.glsl", "fragment_textured.glsl");
	glUseProgram(program);


	positionAttribute = glGetAttribLocation(program, "position");
	colorAttribute = glGetAttribLocation(program, "color");
	timeUniform = glGetUniformLocation(program, "time");
	normalAttribute = glGetAttribLocation(program, "normal");
	texCoordAttribute = glGetAttribLocation(program, "texCoord");
	binormalAttribute = glGetAttribLocation(program, "binormal");
	tangentAttribute = glGetAttribLocation(program, "tangent");

	normalUniformLocation = glGetUniformLocation(program, "normalMatrix");
	colorUniformLocation = glGetUniformLocation(program, "uColor");
	modelviewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");
	diffuseTextureUniformLocation = glGetUniformLocation(program, "diffuseTexture");
	//diffuseTextureUniformLocation1 = glGetUniformLocation(program, "diffuseTexture[1]");
	specularUniformLocation = glGetUniformLocation(program, "specularTexture");
	normalTexUniformLocation = glGetUniformLocation(program, "normalTexture");

	lightDirectionUniformLocation0 = glGetUniformLocation(program, "lights[0].lightDirection");
	lightDirectionUniformLocation1 = glGetUniformLocation(program, "lights[1].lightDirection");
	//lightDirectionUniformLocation1
	//lightDirectionUniform = glGetUniformLocation(program, "lightDirection");


	
	//cube.adoption(&bigCube);
	//bigCube.adoption(&ball);
	//bigCube.adoption(&cube);
	//cube.adoption(&ball);
	//
	//getCubeVbIbLen(vbLen, ibLen);
	//cube.geometry.indexBO = ibLen;
	//cube.geometry.vertexBO = vbLen;
	//cube.geometry.numIndices = ibLen;

	//std::vector<VertexPrime> vtx(vbLen);
	//std::vector<unsigned short> idx(ibLen);

	//makeCube(1, vtx.begin(), idx.begin());


	//getSphereVbIbLen(20, 20, vbLen2, ibLen2); //more slices & stacks make it more smooth and circle like
	//ball.geometry.indexBO = ibLen2;
	//ball.geometry.vertexBO = vbLen2;
	//ball.geometry.numIndices = ibLen2;

	//std::vector<VertexPrime> vtx2(vbLen2);
	//std::vector<unsigned short> idx2(ibLen2);

	//makeSphere(0.5, 20, 20, vtx2.begin(), idx2.begin());

	/*getCubeVbIbLen(vbLen3, ibLen3);
	bigCube.geometry.indexBO = ibLen3;
	bigCube.geometry.vertexBO = vbLen3;
	bigCube.geometry.numIndices = ibLen3;

	std::vector<VertexPrime> vtx3(vbLen3);
	std::vector<unsigned short> idx3(ibLen3);

	makeCube(2, vtx3.begin(), idx3.begin());

	glGenBuffers(1, &bigCube.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, bigCube.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPrime) * vtx3.size(), vtx3.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &bigCube.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bigCube.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx3.size(), idx3.data(), GL_STATIC_DRAW);*/

	

	//areaFloor.binding(diffuseTextureUniformLocation, specularUniformLocation, normalTexUniformLocation);
	
	//std::vector<VertexPrime> meshVertices;
	/*std::vector<VertexPrime> meshVertices;
	std::vector<unsigned short> meshIndices;*/
	loadObjFile("Monk_Giveaway_Fixed.obj", object.meshVertices, object.meshIndices);
	object.geometry.numIndices = object.meshIndices.size();
	object.geometry.vertexBO = object.meshVertices.size();
	object.geometry.indexBO = object.meshIndices.size();

	glGenBuffers(1, &object.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, object.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPrime) * object.meshVertices.size(), object.meshVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &object.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * object.meshIndices.size(), object.meshIndices.data(), GL_STATIC_DRAW);


	object.diffuseTexture = loadGLTexture("Monk_D.tga");
	object.specularTexture = loadGLTexture("Monk_S.tga");
	object.normalTexture = loadGLTexture("Monk_N.tga");
	
	glUniform1i(diffuseTextureUniformLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	//glGenTextures(0, &object.diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, object.diffuseTexture);

	glUniform1i(specularUniformLocation, 1);
	glActiveTexture(GL_TEXTURE1);
	//glGenTextures(1, &object.specularTexture);
	glBindTexture(GL_TEXTURE_2D, object.specularTexture);

	glUniform1i(normalTexUniformLocation, 2);
	glActiveTexture(GL_TEXTURE2);
	//glGenTextures(2, &object.normalTexture);
	glBindTexture(GL_TEXTURE_2D, object.normalTexture);

	//object.binding(diffuseTextureUniformLocation, specularUniformLocation, normalTexUniformLocation);

	loadObjFile("Monk_Giveaway_Fixed.obj", object2.meshVertices, object2.meshIndices);
	object2.geometry.numIndices = object2.meshIndices.size();
	object2.geometry.vertexBO = object2.meshVertices.size();
	object2.geometry.indexBO = object2.meshIndices.size();

	glGenBuffers(1, &object2.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, object2.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPrime) * object2.meshVertices.size(), object2.meshVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &object2.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object2.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * object2.meshIndices.size(), object2.meshIndices.data(), GL_STATIC_DRAW);


	object2.diffuseTexture = loadGLTexture("Monk_D.tga");
	object2.specularTexture = loadGLTexture("Monk_S.tga");
	object2.normalTexture = loadGLTexture("Monk_N.tga");

	glUniform1i(diffuseTextureUniformLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	//glGenTextures(0, &object.diffuseTexture);
	glBindTexture(GL_TEXTURE_2D, object2.diffuseTexture);

	glUniform1i(specularUniformLocation, 1);
	glActiveTexture(GL_TEXTURE1);
	//glGenTextures(1, &object.specularTexture);
	glBindTexture(GL_TEXTURE_2D, object2.specularTexture);

	glUniform1i(normalTexUniformLocation, 2);
	glActiveTexture(GL_TEXTURE2);
	//glGenTextures(2, &object.normalTexture);
	glBindTexture(GL_TEXTURE_2D, object2.normalTexture);

	//object2.binding(diffuseTextureUniformLocation, specularUniformLocation, normalTexUniformLocation);

	
	
	/*getPlaneVbIbLen(vbLen4, ibLen4);
	areaFloor.geometry.indexBO = ibLen4;
	areaFloor.geometry.vertexBO = vbLen4;
	areaFloor.geometry.numIndices = ibLen4;

	std::vector<VertexPrime> vtx4(vbLen4);
	std::vector<unsigned short> idx4(ibLen4);

	makePlane(6, vtx4.begin(), idx4.begin());


	glGenBuffers(1, &areaFloor.geometry.vertexBO);
	glBindBuffer(GL_ARRAY_BUFFER, areaFloor.geometry.vertexBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPrime) * vtx4.size(), vtx4.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &areaFloor.geometry.indexBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, areaFloor.geometry.indexBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx4.size(), idx4.data(), GL_STATIC_DRAW);
*/
	/*areaFloor.diffuseTexture = loadGLTexture("ghost.png");
	areaFloor.specularTexture = loadGLTexture("ghost.png");
	areaFloor.normalTexture = loadGLTexture("ghost.png");*/

	//glUniform1i(diffuseTextureUniformLocation, 0);
	//glActiveTexture(GL_TEXTURE0);
	////glGenTextures(0, &diffuseTexture);
	//glBindTexture(GL_TEXTURE_2D, areaFloor.diffuseTexture);

	//glUniform1i(specularUniformLocation, 1);
	//glActiveTexture(GL_TEXTURE1);
	////glGenTextures(1, &specularTexture);
	//glBindTexture(GL_TEXTURE_2D, areaFloor.specularTexture);

	//glUniform1i(normalTexUniformLocation, 5);
	//glActiveTexture(GL_TEXTURE5);
	////glGenTextures(2, &normalTexture);
	//glBindTexture(GL_TEXTURE_2D, areaFloor.normalTexture);
	//std::vector<VertexPrime> meshVertices2;
	//std::vector<unsigned short> meshIndices2;
	//loadObjFile("Monk_Giveaway.obj", object2.meshVertices, object2.meshIndices);
	//object2.geometry.numIndices = object2.meshIndices.size();
	//object2.geometry.vertexBO = object2.meshVertices.size();
	//object2.geometry.indexBO = object2.meshIndices.size();

	//glGenBuffers(1, &object2.geometry.vertexBO);
	//glBindBuffer(GL_ARRAY_BUFFER, object2.geometry.vertexBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPrime) * object2.meshVertices.size(), object2.meshVertices.data(), GL_STATIC_DRAW);

	//glGenBuffers(1, &object2.geometry.indexBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object2.geometry.indexBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * object2.meshIndices.size(), object2.meshIndices.data(), GL_STATIC_DRAW);


	//object2.diffuseTexture = loadGLTexture("Monk_D.tga");
	//object2.specularTexture = loadGLTexture("Monk_S.tga");
	//object2.normalTexture = loadGLTexture("Monk_N.tga");

	//glUniform1i(diffuseTextureUniformLocation, 0);
	//glActiveTexture(GL_TEXTURE0);
	//////glGenTextures(0, &diffuseTexture);
	//glBindTexture(GL_TEXTURE_2D, object2.diffuseTexture);

	//glUniform1i(specularUniformLocation, 1);
	//glActiveTexture(GL_TEXTURE1);
	//////glGenTextures(1, &specularTexture);
	//glBindTexture(GL_TEXTURE_2D, object2.specularTexture);

	//glUniform1i(normalTexUniformLocation, 2);
	//glActiveTexture(GL_TEXTURE2);
	//////glGenTextures(2, &normalTexture);
	//glBindTexture(GL_TEXTURE_2D, object2.normalTexture);

	//object2.binding(diffuseTextureUniformLocation, specularUniformLocation, normalTexUniformLocation);

	
	//areaFloor.specularTexture = areaFloor.diffuseTexture;
	//areaFloor.normalTexture = areaFloor.diffuseTexture;

	//areaFloor.binding(diffuseTextureUniformLocation, specularUniformLocation, normalTexUniformLocation);


	/*glUniform1i(diffuseTextureUniformLocation, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTexture);*/

	/*glUniform1i(diffuseTextureUniformLocation1, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, areaFloor.texture);*/

	//object.binding(diffuseTextureUniformLocation);
	//areaFloor.binding(diffuseTextureUniformLocation);
	/*glUniform1i(specularUniformLocation, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTexture);

	glUniform1i(normalTexUniformLocation, 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, normalTexture);*/

	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	//glEnableVertexAttribArray(texCoordAttribute);
	glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPrime), (void*)offsetof(VertexPrime, t));
	glEnableVertexAttribArray(texCoordAttribute);

	
	


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