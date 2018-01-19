
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

using namespace std;
using namespace HGLTool;


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


void resizeWindow_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

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


	std::vector<std::shared_ptr<HGLLight>> Lights;
	std::shared_ptr<HGLCameraParallelLight> LightA = std::make_shared<HGLCameraParallelLight>(&viewCamera);
	Lights.push_back(LightA);
	LightA->Diffuse = 0.5f;
	LightA->Specular = 0.5f;
	std::shared_ptr<HGLAmbientLight> LightB = std::make_shared<HGLAmbientLight>();
	Lights.push_back(LightB);
	LightB->Intensity = 0.08f;


	HGLModel model;
	model.SetLightsList(Lights);
	model.CompileShaderWithLights();
	//model.Load("nanosuit/nanosuit.obj");
	model.Load("bones/10.obj");


	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//viewCamera.MoveDeep(-6);
	//viewCamera.MoveVertical(12);
	viewCamera.MoveDeep(-12);

	while (!glfwWindowShouldClose(mainWindow))
	{
		processInput(mainWindow);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model.Draw(viewCamera);

		glfwSwapBuffers(mainWindow);
		glfwPollEvents();
	}

	return 0;
}




void resizeWindow_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


bool MousePushed = false;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		viewCamera.MoveDeep(0.15f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		viewCamera.MoveDeep(-0.15f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		viewCamera.MoveHorizon(-0.15f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		viewCamera.MoveHorizon(0.15f);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
	{
		MousePushed = true;
	}
	else
	{

		MousePushed = false;
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

	if (MousePushed)
	{
		viewCamera.RotateHorizon(glm::radians(-xOffset));
		viewCamera.RotateVertical(glm::radians(yOffset));
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

