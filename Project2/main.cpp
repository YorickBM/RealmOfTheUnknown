// Std. Includes
#include <string>
#include <map>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Collision.h"
#include "ToolBox.h"
#include "ComponentSystemManager.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

//LowPolyMesh Library
#include <LPML/Map.h>
#include <LPML/Mesh.h>

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
const char* TITLE = "Realm Of The Unknown - Beta Release";
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//ComponentSystem
ComponentSystemManager csm;
std::vector<Entity> entities;

#include "RenderObject.h"

/*
Basic Clothing (When no Armor)
Change clothing on armor (Every armor piece looks different)
Give the player a model that loads and that he walks around as!
		V
		Camera Position Seems to be FUCKED UP :?
*/


int main()
{
	#pragma region Window Init
	//Tools->Options->Debugging->Automatically (Last Line)
	//Use above path to toggle if console should auto close or not.

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	#pragma endregion

	// Setup and compile our shaders
	Shader shader("res/shaders/modelLoading.vs", "res/shaders/modelLoading.frag");

	#pragma region ComponentSystem
	csm.Init();

	/* Register The Components & Systems*/
	csm.RegisterComponent<RenderObject>();
	///csm.RegisterSystem<Model>();

	/* Create the Signatures */
	Signature signature;
	signature.set(csm.GetComponentType<RenderObject>());
	///csm.SetSystemSignature<Model>(signature);
	
	auto csmModel = csm.CreateEntity();
	csm.AddComponent(csmModel, RenderObject{ Model("res/models/OBJ/Alien/PineTree3Snowy.obj", glm::vec3(4.7f, 0.f, 0.0f), 0.2f)});
	entities.push_back(csmModel);

	auto csmModel = csm.CreateEntity();
	csm.AddComponent(csmModel, RenderObject{ Model("res/models/OBJ/Evergreen/PineTree1.obj", glm::vec3(4.0f, 0.0f, 0.0f), 0.2f) });
	entities.push_back(csmModel);

	auto csmModel = csm.CreateEntity();
	csm.AddComponent(csmModel, RenderObject{ Model("res/models/OBJ/Pine/PineStump.obj", glm::vec3(0.0f, 0.f, 0.0f), 0.2f) });
	entities.push_back(csmModel);

	auto csmModel = csm.CreateEntity();
	csm.AddComponent(csmModel, RenderObject{ Model("res/models/Rocks/RockBig001.obj", glm::vec3(2.0f, 0.2f, 2.0f), 0.2f) });
	entities.push_back(csmModel);
	#pragma endregion

	Model PlayerModel("res/models/OBJ/Alien/PineTree3Snowy.obj", glm::vec3(2.0f, 0.f, 0.0f), 0.2f);

	// Draw in wireframe
	///glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	///PineTree3Snowy.setPosition(glm::vec3(4.2f, 0.f, 0.0f));
	///PineTree3Snowy.setScale(0.2f);

	std::map<glm::vec3*, Model*> ht;
	///ht[&PineTree3Snowy.getPosition()] = &PineTree3Snowy;
	///ht[&PineTree1.getPosition()] = &PineTree1;
	///ht[&PineStump.getPosition()] = &PineStump;

	camera.SetModel(&PlayerModel);

	int frame = 0;
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Z-Buffer
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		shader.Use();

		//Camera Stuff to Shaders
		camera.Update(shader, projection, ht); //Do projection calculation also inside Camera class?!

		//Just draw model nothin special with pos or scale
		for (auto& entity : entities) {
			auto& Shader = shader;
			auto& Model = csm.GetComponent<RenderObject>(entity).model;

			Model.Draw(Shader);
		}
		///PineTree3Snowy.Draw(shader);
		PlayerModel.Draw(shader);

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#pragma region SystemMovement
// Moves/alters the camera positions based on user input
bool MeshMode = false;
int delay = 0;
void DoMovement()
{
	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}

	#pragma region Polygon Mode Toggle
	if (keys[GLFW_KEY_TAB]) {
		if (!MeshMode && delay == 0) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			MeshMode = true; delay = 10;
			return;
		}else if(delay == 0){
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			MeshMode = false; delay = 10;
			return;
		}
	}
	if (delay > 0) delay--;
	#pragma endregion
}
// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
}
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
#pragma endregion

/*

Object loading in new thread get percentage text on screen :?

-------------------------

Update vbo shit
http://wiki.lwjgl.org/wiki/The_Quad_updating_a_VBO_with_BufferSubData.html

-------------------------

Assimp Animatie (Is in java :?)
https://www.youtube.com/watch?v=fDmMH8_WRok

*/