#include "../bezier/mlbezier.h"
#include "../inc/shader_m.h"
#include "../3rdparty/stb_image.h"
#include "../inc/common.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

bool dragFlag = false;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
mlBezier mlbezier;
GLfloat rotate, time;
int rotate_fac = 15;															// modify here---------rotate parameter
int width, height, nrChannels;


const unsigned int SCR_WINDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;
const int FINE = 100;															// modify here----------fine parameter


glm::mat4 view;
glm::vec3 cameraPos = glm::vec3(6.0f, 0.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
float yaw = 180.0f, pitch = -44.0f;
bool firstMouse = true;
bool flag = true;
float lastX = 800.0f / 2.0;
float lastY = 800.0f / 2.0;
glm::vec3 drawPoint[100][100];
glm::vec3 cp2[25] = {
	{ -2.0, -6.0,  2.0 },
	{ -0.5, -6.0,  1.0 },
	{ 0.5, -6.0, -2.0 },
	{ 2.0, -6.0,  0.0 },
	{ 3.0, -6.0,  1.0 },

	{ -2.0, -5.5,  2.0 },
	{ -0.5, -5.5,  1.5 },
	{ 0.5, -5.5,  0.0 },
	{ 2.0, -5.5, -2.0 },
	{ 3.0, -5.5,  0.0 },

	{ -2.0,  -4.5,  2.0 },
	{ -0.5,  -4.5,  1.0 },
	{ 0.5,  -4.5, -1.0 },
	{ 2.0,  -4.5,  1.0 },
	{ 3.0,  -4.5,  2.0 },

	{ -2.0,  -3.5, 0 },
	{ -0.5,  -3.5, -1.5 },
	{ 0.5,  -3.5,  -4 },
	{ 2.0,  -3.5, 6 },
	{ 3.0,  -3.5, 2 },

	{ -2.0, -2.0,  1.0 },
	{ -0.5, -2.0,  0.0 },
	{ 0.5, -2.0, -2.0 },
	{ 2.0, -2.0,  2.0 },
	{ 3.0, -2.0,  1.0 },
};



// calculate the normal vactor of triangle
void calNorm(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
	glm::vec3 d1, d2;
	d1 = v2 - v1;
	d2 = v3 - v1;
	glNormal3f(glm::cross(d1, d2)[0], glm::cross(d1, d2)[1], glm::cross(d1, d2)[2]);
}


void drawControlPoint(mlBezier &mlbezier)
{
	glBegin(GL_QUADS);
	for (int i = 0; i < (int)mlbezier.indicesofControlpoints.size() / 4; i++)
	{
		glColor3f(0, 1, 0);
		glVertex3f
		(
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i]].x,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i]].y,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i]].z
		);
		glVertex3f
		(
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 1]].x,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 1]].y,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 1]].z
		);
		glVertex3f
		(
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 2]].x,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 2]].y,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 2]].z
		);
		glVertex3f
		(
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 3]].x,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 3]].y,
			mlbezier.controlPoints[mlbezier.indicesofControlpoints[4 * i + 3]].z
		);
	}
	glEnd();
}


// used to replace the original draw_control_point function
void myDrawCPoint() {
	rotate = time * rotate_fac;
	glRotatef(rotate, 0.0f, 1.0f, 0.0f);
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex3f(
				mlbezier.controlPoints[j + i * 5][0],
				mlbezier.controlPoints[j + i * 5][1],
				mlbezier.controlPoints[j + i * 5][2]
			);
			glVertex3f(
				mlbezier.controlPoints[j + i * 5 + 1][0],
				mlbezier.controlPoints[j + i * 5 + 1][1],
				mlbezier.controlPoints[j + i * 5 + 1][2]
			);
			glEnd();
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex3f(
				mlbezier.controlPoints[j + i*5][0],
				mlbezier.controlPoints[j + i*5][1],
				mlbezier.controlPoints[j + i*5][2]
			);
			glVertex3f(
				mlbezier.controlPoints[j + i * 5 + 5][0],
				mlbezier.controlPoints[j + i * 5 + 5][1],
				mlbezier.controlPoints[j + i * 5 + 5][2]
			);
			glEnd();
		}
	}
}

void myDrawCPoint2() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex3f(
				cp2[j + i * 5][0],
				cp2[j + i * 5][1],
				cp2[j + i * 5][2]
			);
			glVertex3f(
				cp2[j + i * 5 + 1][0],
				cp2[j + i * 5 + 1][1],
				cp2[j + i * 5 + 1][2]
			);
			glEnd();
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 5; j++) {
			glBegin(GL_LINES);
			glColor3f(0, 1, 0);
			glVertex3f(
				cp2[j + i * 5][0],
				cp2[j + i * 5][1],
				cp2[j + i * 5][2]
			);
			glVertex3f(
				cp2[j + i * 5 + 5][0],
				cp2[j + i * 5 + 5][1],
				cp2[j + i * 5 + 5][2]
			);
			glEnd();
		}
	}
}

void AddLight()
{
	GLfloat sun_light_position[] = { 0.0f, 0.0f, 10.0f, 1.0f };
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

// algorithm to calculate the bezier curve point
glm::vec3 casteljau(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 p5, GLfloat t) {
	//first order
	glm::vec3 p12 = (1 - t) * p1 + t * p2;
	glm::vec3 p23 = (1 - t) * p2 + t * p3;
	glm::vec3 p34 = (1 - t) * p3 + t * p4;
	glm::vec3 p45 = (1 - t) * p3 + t * p5;
	//second order
	glm::vec3 p123 = (1 - t) * p12 + t * p23;
	glm::vec3 p234 = (1 - t) * p23 + t * p34;
	glm::vec3 p345 = (1 - t) * p34 + t * p45;
	//third order
	glm::vec3 p1234 = (1 - t) * p123 + t * p234;
	glm::vec3 p2345 = (1 - t) * p234 + t * p345;
	return (1 - t) * p1234 + t * p2345;
}


void drawBezierSurface(mlBezier &mlbezier)
{
	glm::vec3 cp[25];
	glm::vec3 ccp[5];

	GLfloat MaterialSpecular[] = { 1.0f, 1.0f ,1.0f, 1.0f };
	GLfloat MaterialDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);

	for (int i = 0; i < 25; i++) {
		cp[i] = mlbezier.controlPoints[i];
	}

	for (int v = 0; v < FINE; v++) {
		for (int u = 0; u < FINE; u++) {
			for (int i = 0; i<5; i++){
				ccp[i] = casteljau(cp[i], cp[i + 5], cp[i + 10], cp[i + 15], cp[i + 20], (GLfloat)v / (FINE-1));
			}
			drawPoint[u][v] = casteljau(ccp[0], ccp[1], ccp[2], ccp[3], ccp[4], (GLfloat)u / (FINE-1));	
		}
	}



	// Draw the quads
	for (int v = 0; v < (FINE-1); v++) {
		for (int u = 0; u < (FINE-1); u++) {

			glBegin(GL_TRIANGLES);
			//glColor3f(1.0f, 1.0f, 0.0f);
			calNorm(drawPoint[u][v], drawPoint[u + 1][v], drawPoint[u + 1][v + 1]);
			glTexCoord2d(0.0f + (float)v / (FINE - 2), 0.0f + (float)u / (FINE - 2));
			glVertex3f(drawPoint[u][v][0], drawPoint[u][v][1], drawPoint[u][v][2]);
			glTexCoord2d(0.0f + (float)v / (FINE - 2), 0.0f + (float)(u+1) / (FINE - 2));
			glVertex3f(drawPoint[u+1][v][0], drawPoint[u+1][v][1], drawPoint[u+1][v][2]);
			glTexCoord2d(0.0f + (float)(v+1) / (FINE - 2), 0.0f + (float)(u+1) / (FINE - 2));
			glVertex3f(drawPoint[u + 1][v+1][0], drawPoint[u + 1][v+1][1], drawPoint[u + 1][v+1][2]);
			glEnd();

			glBegin(GL_TRIANGLES);
			//glColor3f(1.0f, 1.0f, 0.0f);
			calNorm(drawPoint[u][v], drawPoint[u + 1][v + 1], drawPoint[u][v + 1]);
			glTexCoord2d(0.0f + (float)v / (FINE - 2), 0.0f + (float)u / (FINE - 2));
			glVertex3f(drawPoint[u][v][0], drawPoint[u][v][1], drawPoint[u][v][2]);
			glTexCoord2d(0.0f + (float)(v + 1) / (FINE - 2), 0.0f + (float)(u + 1) / (FINE - 2));
			glVertex3f(drawPoint[u + 1][v + 1][0], drawPoint[u + 1][v + 1][1], drawPoint[u + 1][v + 1][2]);
			glTexCoord2d(0.0f + (float)(v + 1) / (FINE - 2), 0.0f + (float)(u) / (FINE - 2));
			glVertex3f(drawPoint[u][v + 1][0], drawPoint[u][v + 1][1], drawPoint[u][v + 1][2]);
			glEnd();
			
		}
	}
}

void drawBezierSurface2(mlBezier &mlbezier)
{
	glm::vec3 cp[25];
	glm::vec3 ccp[5];

	GLfloat MaterialSpecular[] = { 1.0f, 1.0f ,1.0f, 1.0f };
	GLfloat MaterialDiffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat MaterialAmbient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MaterialSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0f);


	for (int i = 0; i < 25; i++) {
		cp[i] = cp2[i];
	}
	for (int v = 0; v < FINE; v++) {
		for (int u = 0; u < FINE; u++) {
			for (int i = 0; i<5; i++) {
				ccp[i] = casteljau(cp[i], cp[i + 5], cp[i + 10], cp[i + 15], cp[i + 20], (GLfloat)v / (FINE - 1));
			}
			drawPoint[u][v] = casteljau(ccp[0], ccp[1], ccp[2], ccp[3], ccp[4], (GLfloat)u / (FINE - 1));
		}
	}
	// Draw the quads
	for (int v = 0; v < (FINE - 1); v++) {
		for (int u = 0; u < (FINE - 1); u++) {

			glBegin(GL_TRIANGLES);
			//glColor3f(1.0f, 1.0f, 0.0f);
			calNorm(drawPoint[u][v], drawPoint[u + 1][v], drawPoint[u + 1][v + 1]);
			glTexCoord2d(0.0f + (float)v / (FINE - 2), 0.0f + (float)u / (FINE - 2));
			glVertex3f(drawPoint[u][v][0], drawPoint[u][v][1], drawPoint[u][v][2]);
			glTexCoord2d(0.0f + (float)v / (FINE - 2), 0.0f + (float)(u + 1) / (FINE - 2));
			glVertex3f(drawPoint[u + 1][v][0], drawPoint[u + 1][v][1], drawPoint[u + 1][v][2]);
			glTexCoord2d(0.0f + (float)(v + 1) / (FINE - 2), 0.0f + (float)(u + 1) / (FINE - 2));
			glVertex3f(drawPoint[u + 1][v + 1][0], drawPoint[u + 1][v + 1][1], drawPoint[u + 1][v + 1][2]);
			glEnd();

			glBegin(GL_TRIANGLES);
			//glColor3f(1.0f, 1.0f, 0.0f);
			calNorm(drawPoint[u][v], drawPoint[u + 1][v + 1], drawPoint[u][v + 1]);
			glTexCoord2d(0.0f + (float)v / (FINE - 2), 0.0f + (float)u / (FINE - 2));
			glVertex3f(drawPoint[u][v][0], drawPoint[u][v][1], drawPoint[u][v][2]);
			glTexCoord2d(0.0f + (float)(v + 1) / (FINE - 2), 0.0f + (float)(u + 1) / (FINE - 2));
			glVertex3f(drawPoint[u + 1][v + 1][0], drawPoint[u + 1][v + 1][1], drawPoint[u + 1][v + 1][2]);
			glTexCoord2d(0.0f + (float)(v + 1) / (FINE - 2), 0.0f + (float)(u) / (FINE - 2));
			glVertex3f(drawPoint[u][v + 1][0], drawPoint[u][v + 1][1], drawPoint[u][v + 1][2]);
			glEnd();

		}
	}
}

void DrawObjectWithTransform() {
	glPushMatrix();
	time = glfwGetTime();
	myDrawCPoint();
	drawBezierSurface(mlbezier);
	myDrawCPoint2();
	drawBezierSurface2(mlbezier);
	glPopMatrix();
}

void initPMV()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, SCR_WINDTH / SCR_HEIGHT, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		cameraUp[0], cameraUp[1], cameraUp[2]
	);
}

void initTexture() {
	unsigned int texture = 0;
	unsigned char *data = stbi_load("resource/textures/container.jpg", &width, &height, &nrChannels, 0);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// sample: specify texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// set the active texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void ChangePmv()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		cameraUp[0], cameraUp[1], cameraUp[2]
	);
	AddLight();  // redefine the positon of the sunlight

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
	AddLight();
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
	cameraFront = front * 100.0f;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt
	(
		cameraPos[0], cameraPos[1], cameraPos[2],
		cameraFront[0], cameraFront[1], cameraFront[2],
		cameraUp[0], cameraUp[1], cameraUp[2]
	);
	AddLight();  // redefine the position of the sunlight
	firstMouse = false;
}


void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.08f;												// ---------modify here
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos -= cameraSpeed * glm::vec3(1.0f, 0.0f, 0.0f);
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


int main()
{
	glfwInit();

	GLFWwindow * window = glfwCreateWindow(SCR_WINDTH, SCR_HEIGHT, "hello", NULL, NULL);
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
	glewInit();
	mlbezier.divs = 36;
	mlbezier.mlCreateBeizermesh();
	mlbezier.mlTriangularization();

	initTexture();

	initPMV();

	AddLight();

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPointSize(4);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		DrawObjectWithTransform();
		
		if (flag) {	// if it's the first time running, preprocess
			preprocessing();
			flag = false;
		}
		ChangePmv();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glfwTerminate();
	return 0;
}