
#include <iostream>
#include "GLAD/glad.h"
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <stb_image.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <memory>

#include "GLTool/GLShader/HGLShader.h"
#include "GLTool/GLTexture2D/HGLTexture2D.h"
#include "GLTool/GLCamera/HGLCamera.h"
#include "GLTool/GLModel/HGLModel.h"
#include "GLTool/GLShader/HGLShaderProgram.h"
#include "GLTool/GLShader/HGLDynamicFragmentShader.h"
#include "GLTool/GLLight/HGLLight.h"
#include "GLTool/GLLight/HGLAmbientLight.h"
#include "GLTool/GLLight/HGLParallelLight.h"
#include "GLTool/GLLight/HGLCameraParallelLight.h"
#include "GLTool/GLLight/HGLPointLight.h"
#include "GLTool/GLLight/HGLSpotLight.h"
#include "GLTool/GLScenes/HGLScenes.h"

using namespace std;
using namespace HGLTool;


// settings
const unsigned int SCR_WIDTH = 500;
const unsigned int SCR_HEIGHT = 500;


void resizeWindow_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
string IntToString(int Param);
void LoadScenseModel(HGLScenes &scense);

HGLCamera viewCamera(glm::radians(45.0f), 1.0f, 0.1f, 200.0f);

int main()
{
	//glfw初始化
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfw新建窗口
	GLFWwindow *mainWindow = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GL", NULL, NULL);
	if (mainWindow == NULL)
	{
		cout << "window create failed!" << endl;
		glfwTerminate();
		return -1;
	}

	//glfw上下文设置
	glfwMakeContextCurrent(mainWindow);

	//glfw设置回掉函数
	glfwSetFramebufferSizeCallback(mainWindow, resizeWindow_callback);
	glfwSetCursorPosCallback(mainWindow, mouse_callback);
	glfwSetScrollCallback(mainWindow, scroll_callback);

	//初始化GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "GLAD initialize failed!" << endl;
	}

	HGLScenes scense;

	std::shared_ptr<HGLCameraParallelLight> LightA = std::make_shared<HGLCameraParallelLight>(&viewCamera);
	LightA->Diffuse = 0.7f;
	LightA->Specular = 0.5f;
	scense.AddLight(LightA);
	std::shared_ptr<HGLAmbientLight> LightB = std::make_shared<HGLAmbientLight>();
	LightB->Intensity = 0.08f;
	scense.AddLight(LightB);

	LoadScenseModel(scense);

	scense.DynamicCompileShader();


	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//viewCamera.MoveDeep(-6);
	//viewCamera.MoveVertical(12);
	viewCamera.MoveDeep(-3);

	while (!glfwWindowShouldClose(mainWindow))
	{
		processInput(mainWindow);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scense.Draw(viewCamera);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	return 0;
}




void resizeWindow_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	viewCamera.Reset(glm::radians(45.0f), (float)width / height, 0.001f, 200.0f);
}


bool leftMousePushed = false;
bool rightMousePushed = false;

void processInput(GLFWwindow *window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{
		leftMousePushed = true;
	}
	else
	{
		leftMousePushed = false;
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		rightMousePushed = true;
	}
	else
	{
		rightMousePushed = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float rate = 0.1f;
	static float lastX = SCR_WIDTH / 2;
	static float lastY = SCR_HEIGHT / 2;

	float xOffset = rate * (xpos - lastX);
	float yOffset = rate * (ypos - lastY);

	lastX = xpos;
	lastY = ypos;

	if (rightMousePushed)
	{
		viewCamera.MoveHorizon(-xOffset * 0.02f);
		viewCamera.MoveVertical(yOffset * 0.02f);
	}
	else if (leftMousePushed)
	{
		glm::mat4 rotationMat(1);
		rotationMat = glm::rotate(rotationMat, glm::radians(-xOffset), glm::vec3(0.0, 1.0, 0.0));
		viewCamera.SetPos(glm::vec3(rotationMat * glm::vec4(viewCamera.GetPos(), 1.0f)));

		viewCamera.SetFront(glm::vec3(0, viewCamera.cameraPos.y, 0) - viewCamera.cameraPos);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	viewCamera.MoveDeep(yoffset / 10.0f);
}


string IntToString(int Param)
{
	stringstream ss;
	ss << Param;
	return ss.str();
}


void LoadScenseModel(HGLScenes &scense)
{
	std::shared_ptr<HGLModel> model[24];
	float offset[24][2] =
	{
		{3.278, 0.174},
		{3.217, 0.159},
		{3.148, 0.145},
		{3.088, 0.152},
		{3.016, 0.163},
		{2.93, 0.198},
		{2.86, 0.22},
		{2.77, 0.247},
		{2.66, 0.273},
		{2.531, 0.31},
		{2.401, 0.324},
		{2.277, 0.335},
		{2.154, 0.367},
		{2.014, 0.354},
		{1.877, 0.327},
		{1.75, 0.273},
		{1.61, 0.206},
		{1.461, 0.164},
		{1.322, 0.129},
		{1.189, 0.115},
		{1.036, 0.115},
		{0.872, 0.15},
		{0.717, 0.196},
		{0.584, 0.243},
	};

	for (int i = 1; i <= 1; ++i)
	{
		glm::mat4 ModelMatrix;
		model[0] = std::make_shared<HGLModel>();
		model[0]->Load("bones/" + IntToString(i) + ".obj");
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		model[0]->SetModelMatrix(ModelMatrix);
		scense.AddModel(model[0]);
		cout << i << endl;
	}
}