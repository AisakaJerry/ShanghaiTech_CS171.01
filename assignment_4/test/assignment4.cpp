#pragma once

#include"../inc/common.h"
#include "../rigid/rigidBody.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

RigidBody rigidbody;

GLuint texGround;
GLuint texCube;

GLuint loadTexture(const char * filename, int width, int height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	// read image
	file = fopen(filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);

	glGenTextures(1, &texture); //generate the texture with the loaded data
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data); //free the texture
	return texture; 
}
void freeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void drawCube() {
	glBindTexture(GL_TEXTURE_2D, texCube); //bind texture 
	glBegin(GL_QUADS);
	// front
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, -halfSize, halfSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, -halfSize, halfSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, halfSize);
	// back
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSize, -halfSize, -halfSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSize, halfSize, -halfSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSize, -halfSize, -halfSize);
	// top
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, -halfSize);
	// bottom
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSize, -halfSize, -halfSize); 
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSize, -halfSize, halfSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSize, -halfSize, halfSize);
	// right
	glTexCoord2f(1.0f, 0.0f); glVertex3f(halfSize, -halfSize, -halfSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(halfSize, halfSize, -halfSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(halfSize, halfSize, halfSize);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(halfSize, -halfSize, halfSize);
	// left
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfSize, -halfSize, -halfSize);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfSize, -halfSize, halfSize);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfSize, halfSize, halfSize);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfSize, halfSize, -halfSize);
	glEnd();
}

void rigidBodyInit()
{

	rigidbody.startFall();

}
 
void drawPlane() {
	glBindTexture(GL_TEXTURE_2D, texGround); //bind texture
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex3d(-2 * size_, 0.0, -2 * size_); 
	glTexCoord2d(1.0, 0.0); glVertex3d(2 * size_, 0.0, -2 * size_); 
	glTexCoord2d(1.0, 1.0); glVertex3d(2 * size_, 0.0, 2 * size_);
	glTexCoord2d(0.0, 1.0); glVertex3d(-2 * size_, 0.0, 2 * size_);
	glEnd();

}






void init() {

	// black background for window

	glShadeModel(GL_SMOOTH);
	glDepthRange(0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_NORMALIZE);
	// glDepthMask(GL_TRUE);

	texGround = loadTexture("rigid/ground.bmp", 512, 512);
	texCube = loadTexture("rigid/mofang.bmp", 256, 256);
	//Setting lights

	GLfloat light0_position[] = { 30.0f, 30.0f, 30.0f, 1.0f }; //spot light
	GLfloat light1_position[] = { -30.0f, -30.0f,-30.0f, 1.0f };
	GLfloat light0_ambient[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light0_diffuse[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat light0_specular[] = { 1.0f, 1.0f, 1.0f, 0.0f };


	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);


	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);


	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);

	rigidBodyInit();

}




void ChangePmv()
{


}

void initPMV()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, SCR_WIDTH / SCR_HEIGHT, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluPerspective(60, SCR_WIDTH / SCR_HEIGHT, 0.1, 100);
	gluLookAt(
		0, 10, 50,
		0, 0, 0,
		0, 1, 0
	);

}

int main(int argc, char* argv[])
{


	glfwInit();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();
	init();
	//initPMV();
	while (!glfwWindowShouldClose(window))
	{
		if ((rigidbody.resetSign == false)||(rigidbody.addForce)) {
			rigidbody.updateFall();
		}
		processInput(window);
		glClearColor(0.3, 0.7, 0.5, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw the camera created in perspective
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60, SCR_WIDTH / SCR_HEIGHT, 0.1, 100);
		gluLookAt(
			0, 10, 30,
			0, 10, 0,
			0, 1, 0
		);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glTranslatef(0, 0, 0);
		glPushMatrix();
		drawPlane();
		glPopMatrix();


		//Setting ball material
		GLfloat ball_mat_ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat ball_mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat ball_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat ball_mat_emission[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat ball_mat_shininess = 10.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ball_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ball_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ball_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, ball_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, ball_mat_shininess);


		glPushMatrix();
		Matrix3x3 m = rigidbody.getStateRotation();
		Vector3d p = rigidbody.getStatePosition();
		GLdouble RM[16] = { m[0][0],m[1][0],m[2][0],0,  m[0][1],m[1][1],m[2][1],0,  m[0][2],m[1][2],m[2][2],0, p.x, p.y, p.z,1 };
		glMultMatrixd(RM);
		//glTranslated(rigidState.xposition.x, rigidState.xposition.y, rigidState.xposition.z);
		//glutSolidCube(size);
		drawCube();
		glPopMatrix();

		glFlush();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;


}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS) {
		rigidbody.addForce = true;
		//std::cout << "Click!" << std::endl;
	}
	else {
		rigidbody.addForce = false;
		//std::cout << "-----" << std::endl;
	}
}

