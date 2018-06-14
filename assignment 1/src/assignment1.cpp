#include "../inc/common.h"
#include "../inc/shader_m.h"
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const GLfloat pi = 3.1415926;

GLfloat rotate;
glm::mat4 view;
glm::vec3 cameraPos		= glm::vec3(6.0f, 0.0f, 6.0f);
glm::vec3 cameraFront	= glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp		= glm::vec3(0.0f, 0.0f, 1.0f);
float yaw = 180.0f, pitch = -44.0f;
bool firstMouse = true;
bool flag = true;
float lastX = 800.0f / 2.0;
float lastY = 800.0f / 2.0;
int rotate_fac = 30;															// ---------modify here


// calculate the normal vactor of triangle
void calNorm(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	glm::vec3 d1, d2;
	d1 = v2 - v1;
	d2 = v3 - v1;
	glNormal3f(glm::cross(d1, d2)[0], glm::cross(d1, d2)[1], glm::cross(d1, d2)[2]);
}

void drawQuad()
{	
	glm::vec3 vv1, vv2, vv3;
	rotate = glfwGetTime() * rotate_fac;
	glRotatef(rotate, -3.0, -3.0, 2*sqrt(2)/3);

	glBegin(GL_TRIANGLES);
	vv1 = glm::vec3(-4, sqrt(3)-3, 0); vv2 = glm::vec3(-4, -sqrt(3) - 3, 0); vv3 = glm::vec3(-1, -3, 0);
	calNorm(vv1, vv2, vv3);
	glVertex3f(-4, sqrt(3) - 3, 0);
	glVertex3f(-4, -sqrt(3) - 3, 0);
	glVertex3f(-1, -3, 0);
	glEnd();
	glBegin(GL_TRIANGLES);
	vv1 = glm::vec3(-4, sqrt(3) - 3, 0); vv2 = glm::vec3(-3, -3, 2 * sqrt(2)); vv3 = glm::vec3(-1, -3, 0);
	calNorm(vv1, vv2, vv3);
	glVertex3f(-4, sqrt(3) - 3, 0);
	glVertex3f(-3, -3, 2 * sqrt(2));
	glVertex3f(-1, -3, 0);
	
	glEnd();
	glBegin(GL_TRIANGLES);
	vv1 = glm::vec3(-4, sqrt(3) - 3, 0); vv2 = glm::vec3(-4, -sqrt(3)-3, 0); vv3 = glm::vec3(-3, -3, 2 * sqrt(2));
	calNorm(vv1, vv2, vv3);
	glVertex3f(-4, sqrt(3) - 3, 0);
	glVertex3f(-4, -sqrt(3) - 3, 0);
	glVertex3f(-3, -3, 2 * sqrt(2));
	glEnd();
	glBegin(GL_TRIANGLES);
	vv1 = glm::vec3(-1, -3, 0); vv2 = glm::vec3(-3, -3, 2 * sqrt(2)); vv3 = glm::vec3(-4, -sqrt(3) - 3, 0);
	calNorm(vv1, vv2, vv3);
	glVertex3f(-1, -3, 0);
	glVertex3f(-3, -3, 2 * sqrt(2));
	glVertex3f(-4, -sqrt(3) - 3, 0);
	glEnd();
}

void drawCube() {

	glEnable(GL_DEPTH_TEST);
	
	rotate = glfwGetTime() * rotate_fac;
	glRotatef(rotate, 0.0, 3.5, 0.0);

	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 4.0f, 0.0f);
	glVertex3f(0.0f, 4.0f, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 4.0f, 1.0f);
	glVertex3f(1.0f, 4.0f, 1.0f);
	glVertex3f(1.0f, 4.0f, 0.0f);
	glVertex3f(0.0f, 4.0f, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 3.0f, 1.0f);
	glVertex3f(0.0f, 3.0f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 3.0f, 1.0f);
	glVertex3f(1.0f, 3.0f, 0.0f);
	glVertex3f(1.0f, 4.0f, 0.0f);
	glVertex3f(1.0f, 4.0f, 1.0f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 0.0f);
	glVertex3f(0.0f, 3.0f, 1.0f);
	glVertex3f(0.0f, 4.0f, 1.0f);
	glVertex3f(0.0f, 4.0f, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 3.0f, 1.0f);
	glVertex3f(1.0f, 3.0f, 1.0f);
	glVertex3f(1.0f, 4.0f, 1.0f);
	glVertex3f(0.0f, 4.0f, 1.0f);
	glEnd();

	//glClear(GL_DEPTH_BUFFER_BIT);
}

int accu = 80;																// ---------modify here
void drawSphere() {
	GLfloat a = 0; GLfloat b = 0; GLfloat c = 0;
	GLfloat r = 1; GLfloat u = 0; GLfloat v = 0;
	GLfloat x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;
	GLfloat step = pi / accu;
	rotate = glfwGetTime() * rotate_fac;											
	glRotatef(rotate, 0.0, 1.0, 0.0);
	for (int i = 0; i < accu*2; i++) {
		v = 0;
		for (int j = 0; j < accu; j++) {
			x1 = r * cos(u) * sin(v);
			y1 = r * cos(v);
			z1 = r * sin(u) * sin(v);
			x2 = r * cos(u) * sin(v + step);
			y2 = r * cos(v + step);
			z2 = r * sin(u) * sin(v + step);
			x3 = r * cos(u + step) * sin(v);
			y3 = r * cos(v);
			z3 = r * sin(u + step) * sin(v);
			x4 = r * cos(u + step) * sin(v + step);
			y4 = r * cos(v + step);
			z4 = r * sin(u + step) * sin(v + step);

			glEnable(GL_DEPTH_TEST);

			glBegin(GL_TRIANGLES);
			//glColor3f(0, x1 / 3, y1 / 3);
			glNormal3f(x1, y1, z1);
			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
			glEnd();

			glBegin(GL_TRIANGLES);
			//glColor3f(0, x2 / 3, y2 / 3);
			glNormal3f(x2, y2, z2);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y3, z3);
			glVertex3f(x4, y4, z4);
			glEnd();

			v += step;
		}
		u += step;
	}
	//glClear(GL_DEPTH_BUFFER_BIT);
}

void addLight() {
	GLfloat sun_light_position[] = { 0.0f, 20.0f, 0.0f, 1.0f };
	GLfloat sun_light_ambient[] = { 0.5f, 0.0f, 0.5f, 1.0f };
	GLfloat sun_light_diffuse[] = { 1.0f, 0.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void initPMV() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, SCR_WIDTH / SCR_HEIGHT, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		cameraUp[0], cameraUp[1], cameraUp[2]
	);
}

void ChangePmv() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		cameraUp[0], cameraUp[1], cameraUp[2]
	);
	addLight();  // redefine the positon of the sunlight

}

void DrawObjectWithTransform() {
	glPushMatrix();
	GLfloat MaterialSpecular[] = { 1.0f, 1.0f ,1.0f, 1.0f };
	GLfloat MaterialDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);

	drawQuad();
	drawCube();
    drawSphere();

	glPopMatrix();
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.08f;												// ---------modify here
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos -= cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
		//cameraFront -= cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += cameraSpeed * glm::vec3(0.0f, 1.0f, 0.0f);
}



// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// to solve the WASD-moving problem, first move mouse for a tiny distance
void preprocessing() {
	glm::vec3 front1;
	front1.x = cos(glm::radians(yaw + 0.00001)) * cos(glm::radians(pitch + 0.00001));
	front1.y = sin(glm::radians(-yaw + 0.00001));
	front1.z = sin(glm::radians(pitch + 0.00001));
	cameraFront = glm::normalize(front1);
	cameraFront = front1 * 100.0f;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		cameraUp[0], cameraUp[1], cameraUp[2]
	);
	addLight();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	
	float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(-yaw)); 
	front.z = sin(glm::radians(pitch));
	cameraFront = glm::normalize(front);
	cameraFront = front*100.0f;
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		cameraUp[0], cameraUp[1], cameraUp[2]
	);
	//std::cout << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;
	addLight();  // redefine the position of the sunlight
	firstMouse = false;
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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_NORMALIZE);

	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	initPMV();

	addLight();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1, 0.1, 0.1, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawObjectWithTransform();
		if (flag) {	// if it's the first time running, preprocess
			preprocessing();
			flag = false;
		}
		ChangePmv();
		std::cout << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;  // output the cameraFront real-time for debug
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}